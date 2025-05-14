#pragma once
#include <vector>
#include <string>
#include <unordered_map>

class Customizer {
private:
    std::unordered_map<int, std::vector<int>> stageEnemyCounts;

public:
    bool LoadFromFile(const std::string& filename);
    int GetEnemyCounter(int stage, int enemyIndex) const;
};
