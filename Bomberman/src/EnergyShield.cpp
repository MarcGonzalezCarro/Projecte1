#include "EnergyShield.h"
#include <iostream>
#include "raylib.h"
#include "Constants.h"

EnergyShield::EnergyShield(Vector2 startPosition)
    : position(startPosition), framecounter(120)// Velocidad predeterminada
{
    currentFrame = 0;
    animFramesCounter = 0;
    framesSpeed = 6;
    isDead = false;
    isActive = true;
}

EnergyShield::~EnergyShield() {}

void EnergyShield::Update(float deltaTime) {
    if (!isDead) {
        framecounter -= deltaTime;
        
        if (framecounter <= 0) {

            framecounter = 120;  // Resetear en segundos, no en frames
        }
    }
    animFramesCounter++;
    if (animFramesCounter >= (60 / framesSpeed)) {
        animFramesCounter = 0;
        currentFrame++;
    }
    if (!isDead) {
        if (currentFrame > 3) {
            currentFrame = 0;
        }
    }
    else {
        if (currentFrame > 2) {
            isActive = false;
        }
    }
    
}


void EnergyShield::Draw() const {
    // Dibujar el coil 
    Vector2 v = { position.x, position.y };
    Rectangle source = { 0, 0, 30, 30 };
    Rectangle dest = { (position.x ), (position.y ), 30 * 8 , 30 * 8 };
    Vector2 v2 = { 1, 1 };
    if (isDead == false) {
        
        source.x = (32 * currentFrame);
    }
    else {
        source.x = (32 * 5 + 32 * currentFrame);
    }
    
    DrawTexturePro(texture, source, dest, v2, 0, WHITE);
    
}


void EnergyShield::SetPosition(Vector2 newPosition) {
    position = newPosition;
}

void EnergyShield::SetTexture(Texture2D newTexture) {
    texture = newTexture;
}



void EnergyShield::Die() {
    isDead = true;
    currentFrame = 0;
}


bool EnergyShield::IsActive()
{
    return isActive;
}
bool EnergyShield::IsDead()
{
    return isDead;
}

