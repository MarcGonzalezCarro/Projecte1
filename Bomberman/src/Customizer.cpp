#include "Customizer.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <regex>

bool Customizer::LoadFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "No se pudo abrir el archivo: " << filename << std::endl;
        return false;
    }

    std::string line;
    std::regex stagePattern(R"(Stage\s+(\d+)\s*-\s*(.*))");
    std::smatch matches;

    while (std::getline(file, line)) {
        if (std::regex_match(line, matches, stagePattern)) {
            int stageNumber = std::stoi(matches[1].str());
            std::string numbersPart = matches[2].str();

            std::vector<int> enemyCounts;
            std::istringstream ss(numbersPart);
            std::string token;

            while (std::getline(ss, token, ',')) {
                try {
                    enemyCounts.push_back(std::stoi(token));
                }
                catch (...) {
                    enemyCounts.push_back(0);
                }
            }

            stageEnemyCounts[stageNumber] = enemyCounts;
        }
    }

    file.close();
    return true;
}

int Customizer::GetEnemyCounter(int stage, int enemyIndex) const {
    auto it = stageEnemyCounts.find(stage);
    if (it != stageEnemyCounts.end()) {
        const std::vector<int>& enemies = it->second;
        if (enemyIndex >= 0 && enemyIndex < (int)enemies.size()) {
            return enemies[enemyIndex];
        }
    }
    return 0;
}
