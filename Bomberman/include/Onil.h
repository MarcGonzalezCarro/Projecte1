#pragma once
#ifndef ONIL_H
#define ONIL_H

#include "EnemyBase.h"

class Onil : public Enemy {
public:
    Onil(Vector2 startPosition);
    ~Onil() override;


private:
    Vector2 direction; // Dirección de movimiento
    float dropBombProbability; // Probabilidad de dejar una bomba
};
#endif