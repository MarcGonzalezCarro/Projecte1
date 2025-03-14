#include "EnemyBase.h"
#include <iostream>
#include "raylib.h"
#include "Constants.h"

Enemy::Enemy(Vector2 startPosition)
    : position(startPosition), speed(1.0f), distance(100), direction({ 1,0 }), framecounter(120)// Velocidad predeterminada
{}

Enemy::~Enemy() {}

void Enemy::Update(float deltaTime, const std::vector<Wall>& walls, const std::vector<SoftBlock>& softblocks) {

    framecounter -= deltaTime;
    if (framecounter <= 0) {
        bool validDirection = false;
        int attempts = 0;

        while (!validDirection && attempts < 10) {
            SetRandomDirection();
            Vector2 nextPosition = { position.x + direction.x * CELL_SIZE, position.y + direction.y * CELL_SIZE };

            if (!CheckWallCollision(nextPosition, walls, softblocks)) {
                validDirection = true;
            }
            attempts++;
        }

        // Si no encontró dirección válida, se queda quieto
        if (!validDirection) {
            direction = { 0, 0 };
            printf("Sa queda quieto xdd");
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


void Enemy::Draw() const {
    // Dibujar el enemigo 
    Vector2 v = { position.x + CAMERA_OFFSET_X, position.y + CAMERA_OFFSET_Y };
    DrawTextureEx(texture, v, 0, 6, WHITE);
}

Vector2 Enemy::GetPosition() const {
    return position;
}

Vector2 Enemy::GetDirection() const{
    return direction;
}

void Enemy::SetPosition(Vector2 newPosition) {
    position = newPosition;
}

void Enemy::SetTexture(Texture2D newTexture) {
    texture = newTexture;
}

void Enemy::SetRandomDirection() {
    int dir = std::rand() % 4; 
    

    switch (dir) {
    case 0: direction = { 1,  0 }; dir = 0; break; // Derecha
    case 1: direction = { -1,  0 }; dir = 0; break; // Izquierda
    case 2: direction = { 0,  1 }; dir = 0; break; // Abajo
    case 3: direction = { 0, -1 }; dir = 0; break; // Arriba
    }

    
}

bool Enemy::CheckWallCollision(Vector2 Position, const std::vector<Wall>& walls, const std::vector<SoftBlock>& softblocks) {
    Rectangle enemyRec = { Position.x, Position.y, CELL_SIZE - 30, CELL_SIZE - 30};

    for (const auto& wall : walls) {
        Rectangle wallRec = wall.GetBound();
        if (CheckCollisionRecs(enemyRec, wallRec)) {
            return true; // Hay colisión
        }
    }
    for (const auto& softBlock : softblocks) {
        Rectangle wallRec = softBlock.GetBound();
        if (CheckCollisionRecs(enemyRec, wallRec)) {
            return true; // Hay colisión
        }
    }
    return false; // No hay colisión
}



