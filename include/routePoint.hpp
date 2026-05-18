//
// Created by Marcel Kosmala on 18/05/2026.
//

#pragma once

namespace Starva {

struct RoutePoint {
    double latitude;
    double longitude;
    float altitude;
    float speed;
    u_int8_t heart_rate;
    u_int8_t cadence;
    u_int16_t power;
    // float grade; chyba useless
    long timestamp;
    // u_int8_t gps_accuracy; // ciekawe czy sie przyda xd
};

}