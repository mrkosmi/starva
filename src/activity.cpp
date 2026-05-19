//
// Created by Marcel Kosmala on 18/05/2026.
//
#include "activity.hpp"

void Starva::Activity::printFrontBack() {
    std::cout << routePoints.front() << std::endl << routePoints.back() << std::endl;
}

std::string Starva::Activity::mappingSourceString() const {
    switch (colorMappingSource) {
        case ColorMappingSource::HeartRate:
            return {"(Heart Rate)"};
        case ColorMappingSource::Speed:
            return {"(Speed)"};
        case ColorMappingSource::Power:
            return {"(Power)"};
        default:
            return {};
    }
}
