#pragma once
#ifndef MINVO_H
#define MINVO_H

#include "EnemyBase.h"

class Minvo : public Enemy {
public:
    Minvo(Vector2 startPosition);
    ~Minvo() override;


private:
    Vector2 direction; // Dirección de movimiento
    float dropBombProbability; // Probabilidad de dejar una bomba
};
#endif