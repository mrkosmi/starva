//
// Created by Marcel Kosmala on 18/05/2026.
//

#pragma once

#include "fit_record_mesg_listener.hpp"
#include "routePoint.hpp"

namespace Starva {

class RecordListener : public fit::RecordMesgListener { // moze jeszcze SessionMesgListener
  std::vector<RoutePoint> points_{};

public:
  RecordListener() = default;
  ~RecordListener() override = default;
  void OnMesg(fit::RecordMesg &recordMesg) override;

  std::vector<RoutePoint>& getPoints() { return points_; }

protected:
  double semicircles2Degrees(int32_t semi);
};

}