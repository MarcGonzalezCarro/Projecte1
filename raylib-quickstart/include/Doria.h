#ifndef DORIA_H
#define DORIA_H

#include "EnemyBase.h"

class Doria : public Enemy {
public:
    Doria(Vector2 startPosition);
    ~Doria() override;


    void Draw() const override;

private:
    Vector2 direction; // Dirección de movimiento
    float dropBombProbability; // Probabilidad de dejar una bomba
    void DropBomb(); // Función para dejar una bomba
};
#endif