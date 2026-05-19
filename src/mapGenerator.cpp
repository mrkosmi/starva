//
// Created by Marcel Kosmala on 19/05/2026.
//
#include "mapGenerator.hpp"
#include <algorithm>

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
}
