#include "Ovape.h"
#include <cstdlib> 
#include <ctime>   
#include "raylib.h"
#include "Constants.h"

Ovape::Ovape(Vector2 startPosition)
    : Enemy(startPosition), direction({ 1, 0 }), dropBombProbability(0.1f)
{
    id = 5;
    score = 2000;

}

Ovape::~Ovape() {}

bool Ovape::CheckWallCollision(Vector2 Position, const std::vector<Wall>& walls, const std::vector<SoftBlock>& softblocks) {
    Rectangle enemyRec = { Position.x, Position.y, CELL_SIZE - 30, CELL_SIZE - 30 };

    for (const auto& wall : walls) {
        Rectangle wallRec = wall.GetBound();
        if (CheckCollisionRecs(enemyRec, wallRec)) {
            return true; // Hay colisión
        }
    }

    return false; // No hay colisión
}