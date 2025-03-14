#ifndef BALLOM_H
#define BALLOM_H

#include "EnemyBase.h"

class Ballom : public Enemy {
public:
    Ballom(Vector2 startPosition);
    ~Ballom() override;

    
    void Draw() const override;

private:
    Vector2 direction; // Dirección de movimiento
    float dropBombProbability; // Probabilidad de dejar una bomba
    void DropBomb(); // Función para dejar una bomba
};
#endif