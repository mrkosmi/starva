//
// Created by Marcel Kosmala on 18/05/2026.
//
#include "activity.hpp"

void Starva::Activity::printFrontBack() {
    std::cout << routePoints.front() << std::endl << routePoints.back() << std::endl;
}
