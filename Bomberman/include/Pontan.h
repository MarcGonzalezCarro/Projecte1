#pragma once
#ifndef PONTAN_H
#define PONTAN_H

#include "EnemyBase.h"

class Pontan : public Enemy {
public:
    Pontan(Vector2 startPosition);
    ~Pontan() override;

    bool CheckWallCollision(Vector2 nextPosition, const std::vector<Wall>& walls, const std::vector<SoftBlock>& softBlocks) override;
    void Update(float deltaTime, const std::vector<Wall>& walls, const std::vector<SoftBlock>& softBlocks) override;



private:
    Vector2 direction; // Dirección de movimiento
    float dropBombProbability; // Probabilidad de dejar una bomba
};
#endif