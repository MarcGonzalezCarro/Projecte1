#pragma once
#ifndef PONTAN_H
#define PONTAN_H

#include "EnemyBase.h"

class Pontan : public Enemy {
public:
    Pontan(Vector2 startPosition);
    ~Pontan() override;



private:
    Vector2 direction; // Dirección de movimiento
    float dropBombProbability; // Probabilidad de dejar una bomba
};
#endif