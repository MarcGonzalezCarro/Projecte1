#include "Pontan.h"
#include <cstdlib> 
#include <ctime>   
#include "raylib.h"
#include "Constants.h"
#include "SoftBlock.h"

Pontan::Pontan(Vector2 startPosition)
    : Enemy(startPosition), direction({ 1, 0 }), dropBombProbability(0.1f)
{
    id = 7;
    score = 8000;

}

Pontan::~Pontan() {}

bool Pontan::CheckWallCollision(Vector2 Position, const std::vector<Wall>& walls, const std::vector<SoftBlock>& softblocks) {
    Rectangle enemyRec = { Position.x, Position.y, CELL_SIZE - 30, CELL_SIZE - 30 };

    for (const auto& wall : walls) {
        Rectangle wallRec = wall.GetBound();
        if (CheckCollisionRecs(enemyRec, wallRec)) {
            return true; // Hay colisión
        }
    }

    return false; // No hay colisión
}

void Pontan::Update(float deltaTime, const std::vector<Wall>& walls, const std::vector<SoftBlock>& softBlocks)
{
    if (!isDead) {
        framecounter -= deltaTime;
        if (direction.x == 0 && direction.y == 0) {
            framecounter = 0;
        }
        if (framecounter <= 0) {
            bool validDirection = false;
            int attempts = 0;

            while (!validDirection && attempts < 10) {
                SetRandomDirection();
                Vector2 nextPosition = { position.x + direction.x * CELL_SIZE, position.y + direction.y * CELL_SIZE };

                if (!CheckWallCollision(nextPosition, walls, softBlocks)) {
                    validDirection = true;
                }
                attempts++;
            }

            // Si no encontró dirección válida, se queda quieto
            if (!validDirection) {
                direction = { 0, 0 };
            }

            framecounter = 120;  // Resetear en segundos, no en frames
            distance = 100;
        }

        if (distance > 0) {
            position.x += direction.x * speed;
            position.y += direction.y * speed;
            distance = distance - speed;
        }
        else {
            distance = 100;
            framecounter = 0;
        }
    }
    animFramesCounter++;
    if (animFramesCounter >= (60 / framesSpeed)) {
        animFramesCounter = 0;
        currentFrame++;
    }
    if (!isDead) {
        if (currentFrame > 1) {
            currentFrame = 0;
        }
    }
    else {
        if (currentFrame > 2) {
            isActive = false;
        }
    }
}
