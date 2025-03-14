#ifndef BALLOM_H
#define BALLOM_H

#include "EnemyBase.h"

class Ballom : public Enemy {
public:
    Ballom(Vector2 startPosition);
    ~Ballom() override;

    
    void Draw() const override;

private:
    Vector2 direction; // Direcci�n de movimiento
    float dropBombProbability; // Probabilidad de dejar una bomba
    void DropBomb(); // Funci�n para dejar una bomba
};
#endif