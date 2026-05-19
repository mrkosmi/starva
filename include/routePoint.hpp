//
// Created by Marcel Kosmala on 18/05/2026.
//

#pragma once

#include <iostream>
#include <format>

namespace Starva {

struct RoutePoint {
    double latitude;
    double longitude;
    long timestamp = 0;
    float altitude = 0;
    float speed = -1; // [mm/s]
    u_int8_t heart_rate = 0; // [bpm]
    bool newHR = false;
    u_int8_t cadence = 0; // [spm]
    u_int16_t power = 0; // [W]
    //float distance = 0;
    // float grade; chyba useless
    // u_int8_t gps_accuracy; // ciekawe czy sie przyda xd

    friend std::ostream &operator<<(std::ostream &out, const RoutePoint &rp);
};

inline std::ostream &operator<<(std::ostream &out, const RoutePoint &rp) {
    out << "Latitude: " << rp.latitude << " Longitude: " << rp.longitude << '\n';
    out << "Speed: " << rp.speed << '\n';
    //out << "Distance: " << rp.distance << '\n';
    out << "Heart-rate: " << static_cast<int>(rp.heart_rate) << (rp.newHR ? '!' : ' ') << '\n';
    out << "Cadence: " << static_cast<int>(rp.cadence) << '\n';
    out << "Power: " << static_cast<int>(rp.power) << '\n';

    return out;
}


}