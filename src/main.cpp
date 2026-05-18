//
// Created by Marcel Kosmala on 17/05/2026.
//

#include <iostream>

#include "fitParser.hpp"

int main(int argc, char* argv[]) {
    std::cout << "Starva - a simple .fit activity visualizer." << std::endl;

    std::filesystem::path current_path = std::filesystem::current_path();

    std::filesystem::path user_path("../examples/10k-run.fit"); // tymczasowo recznie podane bez parsera argumentow
    std::filesystem::path filePath = std::filesystem::absolute(user_path);

    Starva::FitParser parser;
    Starva::Activity activity = parser.parseFile(filePath);

    activity.printFrontBack();
}
