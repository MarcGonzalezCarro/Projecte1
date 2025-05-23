#pragma once
#ifndef OVAPE_H
#define OVAPE_H

#include "EnemyBase.h"

class Ovape : public Enemy {
public:
    Ovape(Vector2 startPosition);
    ~Ovape() override;

    bool CheckWallCollision(Vector2 nextPosition, const std::vector<Wall>& walls, const std::vector<SoftBlock>& softBlocks) override;
private:
    Vector2 direction; // Dirección de movimiento
    float dropBombProbability; // Probabilidad de dejar una bomba
};
#endif