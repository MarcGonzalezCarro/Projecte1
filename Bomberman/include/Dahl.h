#pragma once
#ifndef DAHL_H
#define DAHL_H

#include "EnemyBase.h"

class Dahl : public Enemy {
public:
    Dahl(Vector2 startPosition);
    ~Dahl() override;


private:
    Vector2 direction; // Dirección de movimiento
    float dropBombProbability; // Probabilidad de dejar una bomba
};
#endif