//
// Created by Marcel Kosmala on 18/05/2026.
//

#include "recordListener.hpp"

namespace Starva {
    double RecordListener::semicircles2Degrees(int32_t semi) {
        return semi * 180.0 / std::pow(2, 31);
    }

    void RecordListener::OnMesg(fit::RecordMesg &recordMesg) {
        // float lastDist = 0;
        // short sinceLastDist = 1;
        // double sumSpeedValues = 0;
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

            /*
            if (recordMesg.IsDistanceValid()) {
                newPoint.distance = recordMesg.GetDistance();
                if (newPoint.distance <= points_.back().distance) {
                    size_t s = points_.size();
                    points_.back().distance = (newPoint.distance + points_[s-2].distance) / 2;
                }
            }
            else {
                newPoint.distance = points_.empty() ? 0 : (points_.back().distance + newPoint.speed); // to jest troche logiczne oszustwo ale daje bledy w granicach max 3 metrow wiec wywalone narazie
            }
            */

            if (points_.empty() || !(newPoint.latitude == points_.back().latitude) || !(newPoint.longitude == points_.back().longitude)) {
                std::cout << newPoint << '\n';
                points_.push_back(newPoint);
            }
            return;
        }
        std::cout << "Corrupted record.\n";
    }

}