//
// Created by Marcel Kosmala on 17/05/2026.
//

#include <iostream>
#include <filesystem>
#include <cstdlib>
#include "cli11/CLI11.hpp"

#include "fitParser.hpp"
#include "mapGenerator.hpp"

int main(int argc, char* argv[]) {
    CLI::App starvaApp("Starva - a simple .fit activity visualizer.");

    std::filesystem::path inputPath;
    starvaApp.add_option("inputPath", inputPath, "Path to the input file")
        ->required()
        ->check(CLI::ExistingPath);

    std::string modeString{"none"};
    starvaApp.add_option("-m,--mode", modeString, "Choose working mode")
        ->check(CLI::IsMember({"speed", "power", "heartrate", "none"}));

    std::filesystem::path outputPath{"activity.html"};
    starvaApp.add_option("-o,--output", outputPath, "Path to the output file");

    bool ifShow = false;
    starvaApp.add_flag("-s,--show", ifShow, "Show the generated map");

    CLI11_PARSE(starvaApp, argc, argv);

    std::map<std::string, Starva::ColorMappingSource> map_mode = {
        {"speed", Starva::ColorMappingSource::Speed},
        {"power", Starva::ColorMappingSource::Power},
        {"heartrate", Starva::ColorMappingSource::HeartRate},
        {"none", Starva::ColorMappingSource::None},
    };
    Starva::ColorMappingSource mode = map_mode[modeString];


    std::filesystem::path current_path = std::filesystem::current_path();

    // if (!std::filesystem::exists(inputPath)) {
    //     std::cerr << "File does not exist: " << inputPath << std::endl;
    //     return 1;
    // }

    if (std::filesystem::exists(outputPath)) {
        char answer;
        std::cout << "File " << outputPath << " already exists. Override? [y/n]" << std::endl;
        std::cin >> answer;
        if (std::tolower(answer) == 'n') return 1;
    }

    Starva::Activity activity;
    try {
        Starva::FitParser parser;
        activity = parser.parseFile(inputPath);
    }
    catch (std::exception& e) {
        std::cerr << "Error decoding file: " << e.what() << std::endl;
        return 1;
    }

    activity.setColorMappingSource(mode);

    // activity.setColorMappingSource(Starva::ColorMappingSource::Speed); // tez tymczasowo hardcoded bez parsera
    // activity.printFrontBack();

    Starva::MapGenerator mapGenerator(activity);

    //std::filesystem::path user_outPath("../mapa.html"); // tymczasowo recznie podane bez parsera argumentow

    mapGenerator.generate(outputPath);
    std::cout << "Written to " << outputPath << "." << std::endl;

    if (ifShow) {
        std::string command;

        #if defined(_WIN32)
                // Windows
                // Używamy "start", puste cudzysłowy "" to sztuczka zapobiegająca
                // błędom, gdy ścieżka do pliku zawiera spacje.
                command = "start \"\" \"" + filepath + "\"";

        #elif defined(__APPLE__)
                // macOS
                command = "open \"" + std::string(outputPath) + "\"";

        #elif defined(__linux__) || defined(__unix__)
                // Linux i inne systemy uniksowe
                command = "xdg-open \"" + std::string(outputPath) + "\"";

        #else
                std::cerr << "Nieobsługiwany system operacyjny. Otwórz plik ręcznie: " << std::string(outputPath) << std::endl;
                return 1;
        #endif

                // Wykonanie komendy w konsoli systemowej
                int result = std::system(command.c_str());

                if (result != 0) {
                    std::cerr << "Ostrzeżenie: Nie udało się automatycznie otworzyć pliku." << std::endl;
                    return 1;
                }
    }

    return 0;
}
