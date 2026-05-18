//
// Created by Marcel Kosmala on 18/05/2026.
//

#pragma once

#include <vector>
#include "routePoint.hpp"

namespace Starva {

    class Activity {
        std::vector<RoutePoint> routePoints;
    public:
        Activity() = default;
        Activity(std::vector<RoutePoint>&& routePoints) : routePoints(std::move(routePoints)) {}
        ~Activity() = default;

        std::vector<RoutePoint> getRoutePoints() { return routePoints; }
        void printFrontBack();
    };

}