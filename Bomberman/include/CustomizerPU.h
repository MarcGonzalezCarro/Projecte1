#pragma once
#include <string>
#include <map>

class CustomizerPU {
public:
    bool LoadFromFile(const std::string& filename);
    int GetPowerUpForStage(int stage) const;

private:
    std::map<int, int> stagePowerUp; // stage -> power-up index (0 a 7)
};