//
// Created by Marcel Kosmala on 17/05/2026.
//

#include <iostream>
#include <filesystem>

#include "fitParser.hpp"
#include "mapGenerator.hpp"

int main(int argc, char* argv[]) {
    std::cout << "Starva - a simple .fit activity visualizer." << std::endl;

    std::filesystem::path current_path = std::filesystem::current_path();

    std::filesystem::path user_path("../examples/interval-run.fit"); // tymczasowo recznie podane bez parsera argumentow
    std::filesystem::path filePath = std::filesystem::absolute(user_path);

    if (!std::filesystem::exists(filePath)) {
        std::cerr << "File does not exist: " << filePath << std::endl;
        return 1;
    }

    Starva::FitParser parser;
    Starva::Activity activity = parser.parseFile(filePath);

    activity.setColorMappingSource(Starva::ColorMappingSource::Speed); // tez tymczasowo hardcoded bez parsera
    activity.printFrontBack();

    Starva::MapGenerator mapGenerator(activity);
    std::filesystem::path user_outPath("../mapa.html"); // tymczasowo recznie podane bez parsera argumentow
    std::filesystem::path outputPath = std::filesystem::absolute(user_outPath);
    std::cout << outputPath << std::endl;

    mapGenerator.generate(outputPath);

    return 0;
}
