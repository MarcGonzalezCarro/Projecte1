#include "BossBase.h"
#include <iostream>
#include "raylib.h"
#include "Constants.h"

Boss::Boss(Vector2 startPosition)
    : position(startPosition), speed(1.0f), distance(100), direction({ 0,0 }), framecounter(120)// Velocidad predeterminada
{
    currentFrame = 0;
    animFramesCounter = 0;
    framesSpeed = 6;
    isDead = false;
    isActive = true;
}

Boss::~Boss() {}

void Boss::Phase1Attack(Vector2 pos)
{
}

void Boss::Update(float deltaTime, const std::vector<Wall>& walls, const std::vector<SoftBlock>& softblocks) {
    
}


void Boss::Draw() const {
    
}

Vector2 Boss::GetPosition() const {
    return position;
}

Vector2 Boss::GetDirection() const{
    return direction;
}

int Boss::GetScore()
{
    return score;
}

void Boss::SetPosition(Vector2 newPosition) {
    position = newPosition;
}

void Boss::SetTexture(Texture2D newTexture) {
    texture = newTexture;
}

void Boss::SetRandomDirection() {
    int dir = std::rand() % 4; 
    

    switch (dir) {
    case 0: direction = { 1,  0 }; dir = 0; break; // Derecha
    case 1: direction = { -1,  0 }; dir = 0; break; // Izquierda
    case 2: direction = { 0,  1 }; dir = 0; break; // Abajo
    case 3: direction = { 0, -1 }; dir = 0; break; // Arriba
    }

    
}

bool Boss::CheckWallCollision(Vector2 Position, const std::vector<Wall>& walls, const std::vector<SoftBlock>& softblocks) {
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

void Boss::Die() {
    isDead = true;
    currentFrame = 0;
}

void Boss::SetSpeed(float temp) {
    speed = temp;
}

bool Boss::IsActive()
{
    return isActive;
}
bool Boss::IsDead()
{
    return isDead;
}

