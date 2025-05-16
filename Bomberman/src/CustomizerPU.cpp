#include "CustomizerPU.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <regex>

bool CustomizerPU::LoadFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "No es pot obrir el fitxer: " << filename << std::endl;
        return false;
    }

    std::string line;
    std::regex stagePattern(R"(Stage\s+(\d+)\s*-\s*(\d+))");
    std::smatch matches;

    while (std::getline(file, line)) {
        if (std::regex_match(line, matches, stagePattern)) {
            int stageNumber = std::stoi(matches[1].str());
            int powerUpIndex = std::stoi(matches[2].str());

            // Validem que el power-up estigui entre 0 i 7
            if (powerUpIndex >= 0 && powerUpIndex < 8) {
                stagePowerUp[stageNumber] = powerUpIndex;
            }
            else {
                std::cerr << "Power-up invàlid al Stage " << stageNumber << ": " << powerUpIndex << std::endl;
            }
        }
    }

    file.close();
    return true;
}

int CustomizerPU::GetPowerUpForStage(int stage) const {
    auto it = stagePowerUp.find(stage);
    if (it != stagePowerUp.end()) {
        return it->second;
    }
    return -1; // Retornem -1 si no hi ha cap power-up assignat a aquest stage
}
