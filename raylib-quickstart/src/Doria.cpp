#include "Doria.h"
#include <cstdlib> 
#include <ctime>   
#include "raylib.h"
#include "Constants.h"

Doria::Doria(Vector2 startPosition)
    : Enemy(startPosition), direction({ 1, 0 }), dropBombProbability(0.1f)
{
    id = 1;


}

Doria::~Doria() {}

void Doria::DropBomb() {

}

bool Doria::CheckWallCollision(Vector2 Position, const std::vector<Wall>& walls, const std::vector<SoftBlock>& softblocks) {
    Rectangle enemyRec = { Position.x, Position.y, CELL_SIZE - 30, CELL_SIZE - 30 };
    
    for (const auto& wall : walls) {
        Rectangle wallRec = wall.GetBound();
        if (CheckCollisionRecs(enemyRec, wallRec)) {
            return true; // Hay colisión
        }
    }
    
    return false; // No hay colisión
}