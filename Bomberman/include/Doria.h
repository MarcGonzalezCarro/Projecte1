#ifndef DORIA_H
#define DORIA_H

#include "EnemyBase.h"

class Doria : public Enemy {
public:
    Doria(Vector2 startPosition);
    ~Doria() override;

    
    bool CheckWallCollision(Vector2 nextPosition, const std::vector<Wall>& walls, const std::vector<SoftBlock>& softBlocks) override;

private:
    Vector2 direction; // Dirección de movimiento
    float dropBombProbability; // Probabilidad de dejar una bomba
};
#endif