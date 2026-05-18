//
// Created by Marcel Kosmala on 18/05/2026.
//

#include "recordListener.hpp"

namespace Starva {
    double RecordListener::semicircles2Degrees(int32_t semi) {
        return semi * 180.0 / std::pow(2, 31);
    }

    void RecordListener::OnMesg(fit::RecordMesg &recordMesg) {
    if (recordMesg.IsPositionLatValid() && recordMesg.IsPositionLongValid()) {
        RoutePoint newPoint{};
        newPoint.latitude = semicircles2Degrees(recordMesg.GetPositionLat());
        newPoint.longitude = semicircles2Degrees(recordMesg.GetPositionLong());
        // newPoint.timestamp = recordMesg.GetTimestamp(); // w sumie chyba narazie useless

        if (recordMesg.IsEnhancedSpeedValid()) {
            newPoint.speed = recordMesg.GetEnhancedSpeed();
        }
        else if (recordMesg.IsSpeedValid()) {
            newPoint.speed = recordMesg.GetSpeed();
        }

        if (recordMesg.IsHeartRateValid()) {
            newPoint.heart_rate = recordMesg.GetHeartRate();
            newPoint.newHR = true;
        }
        else {
            newPoint.heart_rate = !points_.empty() ? points_.back().heart_rate : 0;
        }

        if (recordMesg.IsCadenceValid()) {
            newPoint.cadence = recordMesg.GetCadence();
        }

        if (recordMesg.IsPowerValid()) {
            newPoint.power = recordMesg.GetPower();
        }

        std::cout << newPoint << '\n';
        points_.push_back(newPoint);
        return;
    }
    std::cout << "Corrupted record.\n";
}

}