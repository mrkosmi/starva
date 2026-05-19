//
// Created by Marcel Kosmala on 19/05/2026.
//
#include "mapGenerator.hpp"
#include <algorithm>
#include <fstream>
#include <nlohmann/json.hpp>
#include <format>

namespace Starva {
    std::vector<MapGenerator::MapPoint> MapGenerator::prepareMapPoints() {
        auto points = activity_.getRoutePoints();

        auto getSourceVal = [&](const RoutePoint& point) {
            switch (activity_.getColorMappingSource()) {
                case ColorMappingSource::Speed: return static_cast<double>(point.speed);
                case ColorMappingSource::HeartRate: return static_cast<double>(point.heart_rate);
                case ColorMappingSource::Power: return static_cast<double>(point.power);
                default: return 0.0;
            }
        };

        auto [min_it, max_it] = std::minmax_element(
                points.begin(),
                points.end(),
                [&](const RoutePoint& p1, const RoutePoint& p2) {
                    return (getSourceVal(p1) < getSourceVal(p2));
                }
        );

        auto minval = getSourceVal(*min_it);
        auto maxval = getSourceVal(*max_it);
        auto roznica = maxval - minval;

        const float totalDistance = activity_.totalDistance();
        if (totalDistance==0.0) throw std::runtime_error("distance=0: would have to divide by 0 lol");

        std::vector<MapPoint> mapPoints{};
        for (const RoutePoint& point : points) {
            double sv = getSourceVal(point);
            double normalized = (roznica==0) ? 0.5 : (sv - minval) / roznica;
            double progress = point.distance / totalDistance;
            mapPoints.push_back({point.latitude, point.longitude, normalized, progress});
        }

        return mapPoints;
    }

    MapGenerator::Color MapGenerator::getColor(double weight) {
        weight = std::clamp(weight, 0.0, 1.0);

        struct Stop {double t; u_int8_t r,g,b;};
        const std::vector<Stop> stops = {
            {0.0, 15, 40, 250},
            {0.25, 130, 60, 180},
            {0.5, 250, 50, 30},
            {0.75, 250, 180, 20},
            {1.0, 255, 255, 180}
        };

        for (size_t i=0; i<stops.size()-1; ++i) {
            if (weight >= stops[i].t && weight <= stops[i+1].t) {
                double t = (weight-stops[i].t) / (stops[i+1].t-stops[i].t);
                return {
                    static_cast<u_int8_t>(stops[i].r + t*(stops[i+1].r - stops[i].r)),
                    static_cast<u_int8_t>(stops[i].g + t*(stops[i+1].g - stops[i].g)),
                    static_cast<u_int8_t>(stops[i].b + t*(stops[i+1].b - stops[i].b))
                    };
            }
        }

        return {static_cast<u_int8_t>(255), static_cast<u_int8_t>(255), static_cast<u_int8_t>(255)}; // mozna przeanalizowa czy to w ogole wstapi bo jest clamp ale warnign wyskakuje bez tego
    }

    std::string MapGenerator::Color::toHex() {
        std::string hex = "#";
        hex += std::format("{:02X}", r);
        hex += std::format("{:02X}", g);
        hex += std::format("{:02X}", b);
        return hex;
    }

    void MapGenerator::generate(const std::filesystem::path &output_path) {
        using json = nlohmann::json;
        auto mapPoints = prepareMapPoints();
        if (mapPoints.empty()) throw std::runtime_error("Something went wrong (mapPoints is empty)"); // to sie chyba nie moze wydarzyc, mzona przeanalizowac

        json coordinates = json::array();
        for (const auto& point : mapPoints) {
            coordinates.push_back({point.longitude, point.latitude});
        }

        json gradient = json::array();
        gradient.push_back("interpolate");
        gradient.push_back(json::array({"linear"}));
        gradient.push_back(json::array({"line-progress"}));

        for (const auto& point : mapPoints) {
            gradient.push_back(point.progress);
            gradient.push_back(getColor(point.weight).toHex()); // TODO: toHex()
        }

        std::string html = "placeholder";
        /* WZÓR Z https://maplibre.org/maplibre-gl-js/docs/examples/create-a-gradient-line-using-an-expression/#__tabbed_1_2
<!DOCTYPE html>
<html lang="en">
<head>
    <title>Create a gradient line using an expression</title>
    <meta property="og:description" content="Use the line-gradient paint property and an expression to visualize distance from the starting point of a line." />
    <meta property="og:created" content="2025-06-25" />
    <meta charset='utf-8'>
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <link rel='stylesheet' href='https://unpkg.com/maplibre-gl@5.24.0/dist/maplibre-gl.css' />
    <script src='https://unpkg.com/maplibre-gl@5.24.0/dist/maplibre-gl.js'></script>
    <style>
        body { margin: 0; padding: 0; }
        html, body, #map { height: 100%; }
    </style>
</head>
<body>
<div id="map"></div>

<script>
    const map = new maplibregl.Map({
        container: 'map',
        style: 'https://tiles.openfreemap.org/styles/bright',
        center: [-77.035, 38.875],
        zoom: 12
    });

    const geojson = {
        'type': 'FeatureCollection',
        'features': [
            {
                'type': 'Feature',
                'properties': {},
                'geometry': {
                    'coordinates': [
                        [-77.044211, 38.852924],
                        [-77.045659, 38.860158],
                        [-77.044232, 38.862326],
                        [-77.040879, 38.865454],
                        [-77.039936, 38.867698],
                        [-77.040338, 38.86943],
                        [-77.04264, 38.872528],
                        [-77.03696, 38.878424],
                        [-77.032309, 38.87937],
                        [-77.030056, 38.880945],
                        [-77.027645, 38.881779],
                        [-77.026946, 38.882645],
                        [-77.026942, 38.885502],
                        [-77.028054, 38.887449],
                        [-77.02806, 38.892088],
                        [-77.03364, 38.892108],
                        [-77.033643, 38.899926]
                    ],
                    'type': 'LineString'
                }
            }
        ]
    };

    map.on('load', () => {
        // 'line-gradient' can only be used with GeoJSON sources
        // and the source must have the 'lineMetrics' option set to true
        map.addSource('line', {
            type: 'geojson',
            lineMetrics: true,
            data: geojson
        });

        // the layer must be of type 'line'
        map.addLayer({
            type: 'line',
            source: 'line',
            id: 'line',
            paint: {
                'line-color': 'red',
                'line-width': 14,
                // 'line-gradient' must be specified using an expression
                // with the special 'line-progress' property
                'line-gradient': [
                    'interpolate',
                    ['linear'],
                    ['line-progress'],
                    0,
                    'blue',
                    0.1,
                    'royalblue',
                    0.3,
                    'cyan',
                    0.5,
                    'lime',
                    0.7,
                    'yellow',
                    1,
                    'red'
                ]
            },
            layout: {
                'line-cap': 'round',
                'line-join': 'round'
            }
        });
    });
</script>
</body>
</html>
*/

        if (std::filesystem::exists(output_path)) {
            throw std::runtime_error("czy ktos jest pewny ze chce nadpisac trzeba zapytac w przyszlosci");
        }

        std::ofstream out(output_path);
        out << html;
    }
}
