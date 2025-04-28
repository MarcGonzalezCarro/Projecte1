#pragma once
#ifndef PASS_H
#define PASS_H

#include "EnemyBase.h"

class Pass : public Enemy {
public:
    Pass(Vector2 startPosition);
    ~Pass() override;


private:
    Vector2 direction; // Direcci�n de movimiento
    float dropBombProbability; // Probabilidad de dejar una bomba
};
#endif