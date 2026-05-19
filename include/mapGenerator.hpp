//
// Created by Marcel Kosmala on 19/05/2026.
//
#pragma once
#include <filesystem>

#include "activity.hpp"

namespace Starva {
    class MapGenerator {
    public:
        struct Color {
            uint8_t r, g, b;
            std::string toHex();
        };

        explicit MapGenerator(const Activity& activity) : activity_(activity) {}
        void generate(const std::filesystem::path& output_path);

    private:
        struct MapPoint {
            double latitude;
            double longitude;
            double weight;
            double progress;
        };

        const Activity& activity_;

        std::vector<MapPoint> prepareMapPoints();
        Color getColor(double weight);
        double calcDist(const RoutePoint& p1, const RoutePoint& p2) const;
    };
}
