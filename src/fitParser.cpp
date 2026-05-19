//
// Created by Marcel Kosmala on 18/05/2026.
//
#include "fitParser.hpp"

#include "recordListener.hpp"
#include <fstream>

namespace Starva {
    Activity FitParser::parseFile(const std::filesystem::path& path) {
        fit::Decode decode;
        fit::MesgBroadcaster mesgBroadcaster;

        RecordListener recordListener;
        std::fstream file;

        file.open(path, std::ios::in | std::ios::binary);
        if (!file.is_open()) {
            std::cerr << "Error opening file: " << path << std::endl;
            throw std::runtime_error("Error opening file");
            // jakis blad moznaby wywalic
        }

        mesgBroadcaster.AddListener(recordListener);

        try {
            decode.Read(file, mesgBroadcaster);
        } catch (const fit::RuntimeException& e) {
            std::cerr << "Exception decoding file: " << e.what() << std::endl;
        }

        return Activity(std::move(recordListener.getPoints()));
    }
}
