#include "MechaBlastX.h"
#include <iostream>
#include "raylib.h"
#include "Constants.h"

MechaBlastX::MechaBlastX(Vector2 startPosition)
    : position(startPosition), speed(1.0f), distance(100), direction({ 0,0 }), framecounter(120)// Velocidad predeterminada
{
    
}

MechaBlastX::~MechaBlastX() {}

void MechaBlastX::Update(float deltaTime, const std::vector<Wall>& walls, const std::vector<SoftBlock>& softblocks) {
    
}


void MechaBlastX::Draw() const {
    
}

Vector2 MechaBlastX::GetPosition() const {
    return position;
}

Vector2 MechaBlastX::GetDirection() const{
    return direction;
}

int MechaBlastX::GetScore()
{
    return score;
}

void MechaBlastX::SetPosition(Vector2 newPosition) {
    position = newPosition;
}

void MechaBlastX::SetTexture(Texture2D newTexture) {
    texture = newTexture;
}

void MechaBlastX::SetRandomDirection() {
    int dir = std::rand() % 4; 
    

    switch (dir) {
    case 0: direction = { 1,  0 }; dir = 0; break; // Derecha
    case 1: direction = { -1,  0 }; dir = 0; break; // Izquierda
    case 2: direction = { 0,  1 }; dir = 0; break; // Abajo
    case 3: direction = { 0, -1 }; dir = 0; break; // Arriba
    }

    
}

bool MechaBlastX::CheckWallCollision(Vector2 Position, const std::vector<Wall>& walls, const std::vector<SoftBlock>& softblocks) {
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

void MechaBlastX::Die() {
    isDead = true;
    currentFrame = 0;
}

void MechaBlastX::SetSpeed(float temp) {
    speed = temp;
}

bool MechaBlastX::IsActive()
{
    return isActive;
}
bool MechaBlastX::IsDead()
{
    return isDead;
}

