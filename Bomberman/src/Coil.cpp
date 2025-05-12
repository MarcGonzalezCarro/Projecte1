#include "Coil.h"
#include <iostream>
#include "raylib.h"
#include "Constants.h"

Coil::Coil(Vector2 startPosition)
    : position(startPosition), framecounter(120)// Velocidad predeterminada
{
    currentFrame = 0;
    animFramesCounter = 0;
    framesSpeed = 6;
    isDead = false;
    isActive = true;
}

Coil::~Coil() {}

void Coil::Update(float deltaTime) {
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


void Coil::Draw() const {
    // Dibujar el coil 
    Vector2 v = { position.x, position.y };
    Rectangle source = { 0, 0, 32, 68 };
    Rectangle dest = { (position.x ), (position.y ), 32 * 4.3f , 68 * 4.3f };
    Vector2 v2 = { 1, 1 };
    if (isDead == false) {
        
        source.x = (32 * currentFrame);
    }
    else {
        source.x = (32 * 5 + 32 * currentFrame);
    }
    
    DrawTexturePro(texture, source, dest, v2, 0, WHITE);
    
}


void Coil::SetPosition(Vector2 newPosition) {
    position = newPosition;
}

void Coil::SetTexture(Texture2D newTexture) {
    texture = newTexture;
}



void Coil::Die() {
    isDead = true;
    currentFrame = 0;
}


bool Coil::IsActive()
{
    return isActive;
}
bool Coil::IsDead()
{
    return isDead;
}

