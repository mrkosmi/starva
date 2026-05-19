//
// Created by Marcel Kosmala on 18/05/2026.
//

#pragma once

#include "activity.hpp"
#include "fit_mesg_broadcaster.hpp"
#include "fit_record_mesg.hpp"
#include <filesystem>

namespace Starva {

class FitParser {

public:
    FitParser() = default;
    ~FitParser() = default;

    Activity parseFile(const std::filesystem::path& path);
};

}