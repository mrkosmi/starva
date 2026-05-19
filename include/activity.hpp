//
// Created by Marcel Kosmala on 18/05/2026.
//

#pragma once

#include <vector>
#include "routePoint.hpp"

namespace Starva {

    enum class ColorMappingSource {
        Speed,
        HeartRate,
        Power,
        None
    };

    class Activity {
        std::vector<RoutePoint> routePoints;
        ColorMappingSource colorMappingSource = ColorMappingSource::None;
    public:
        Activity() = default;
        explicit Activity(std::vector<RoutePoint>&& routePoints) : routePoints(std::move(routePoints)) {}
        ~Activity() = default;

        void setColorMappingSource(ColorMappingSource source) { colorMappingSource = source; }
        [[nodiscard]] const ColorMappingSource& getColorMappingSource() const { return colorMappingSource; }

        [[nodiscard]] const std::vector<RoutePoint>& getRoutePoints() const { return routePoints; }
        void printFrontBack();

        [[nodiscard]] float totalDistance() const { return routePoints.back().distance; }
    };

}