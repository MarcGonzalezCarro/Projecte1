#include "EnemyBase.h"
#include <iostream>
#include "raylib.h"
#include "Constants.h"

Enemy::Enemy(Vector2 startPosition)
    : position(startPosition), speed(1.0f), distance(100), direction({ 1,0 }), framecounter(120)// Velocidad predeterminada
{
    currentFrame = 0;
    animFramesCounter = 0;
    framesSpeed = 6;
    isDead = false;
}

Enemy::~Enemy() {}

void Enemy::Update(float deltaTime, const std::vector<Wall>& walls, const std::vector<SoftBlock>& softblocks) {
    if (!isDead) {
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
        if (currentFrame > 2) {
            currentFrame = 0;
        }
    }
    else {
        if (currentFrame > 2) {
            isActive = false;
        }
    }
    
}


void Enemy::Draw() const {
    // Dibujar el enemigo 
    Vector2 v = { position.x + CAMERA_OFFSET_X, position.y + CAMERA_OFFSET_Y };
    Rectangle source = { 0, 0, SPRITE_SIZE, SPRITE_SIZE };
    Rectangle dest = { position.x + CAMERA_OFFSET_X, position.y + CAMERA_OFFSET_Y, SPRITE_SIZE * 6, SPRITE_SIZE * 6 };
    Vector2 v2 = { 1, 1 };
    if (isDead == false) {
        if (direction.x == 1 && direction.y == 0) {
            source.x = SPRITE_SIZE * currentFrame;
        }
        if (direction.x == -1 && direction.y == 0) {
            source.x = SPRITE_SIZE * 2 + SPRITE_SIZE * currentFrame;
        }
        if (direction.x == 0 && direction.y == 1) {
            source.x = SPRITE_SIZE * currentFrame;
        }
        if (direction.x == 0 && direction.y == -1) {
            source.x = SPRITE_SIZE * 2 + SPRITE_SIZE * currentFrame;
        }
    }
    else {
        source.x = SPRITE_SIZE * 5 + SPRITE_SIZE * currentFrame;
    }
    DrawTexturePro(texture, source, dest, v2, 0, WHITE);
    
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

void Enemy::Die() {
    isDead = true;
    currentFrame = 0;
}

void Enemy::SetSpeed(float temp) {
    speed = temp;
}

bool Enemy::IsActive()
{
    return isActive;
}
bool Enemy::IsDead()
{
    return isDead;
}

