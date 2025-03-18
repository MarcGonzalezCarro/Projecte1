#include "Bomb.h"
#include "Constants.h"
#include "Blast.h"
#include <vector>

extern std::vector<Blast> blasts; 

Bomb::Bomb(int startX, int startY) : x(startX), y(startY), active(true), timer(3.0f), range(8) {
    
}

Bomb::~Bomb() {
    UnloadTexture(texture);
}

float Bomb::Update(float deltaTime) {
    if (active) {
        timer -= deltaTime;
        
    }
    return timer;
}
void Bomb::Explode() {

    Vector2 v = { x,y };
    blasts.push_back(Blast(v));
    
}
void Bomb::Draw() const {
    
    if (active) {
        Vector2 v = { x + CAMERA_OFFSET_X, y + CAMERA_OFFSET_Y};
        DrawTextureEx(texture, v, 0, 6, WHITE);
    }
     
}

bool Bomb::HasExploded() const {
    return !active && timer <= 0;
}

Rectangle Bomb::GetBounds() const {
    return { (float)x, (float)y, 40, 40 };
}

void Bomb::Activate(int newX, int newY) {
    if (!active) {
        x = newX;
        y = newY;
        active = true;
        timer = 3.0f;  // Reiniciar el temporizador
    }
}

bool Bomb::IsActive() const {
    return active;
}

int Bomb::GetX() const {
    return x;
}

int Bomb::GetY() const {
    return y;
}
int Bomb::GetRange() const {
    return range;
}

void Bomb::SetTexture(Texture2D tex) {
    texture = tex;
}