#include "Bomb.h"
#include "Constants.h"
#include "Blast.h"
#include <vector>

extern std::vector<Blast> blasts; 

Bomb::Bomb(int startX, int startY) : x(startX), y(startY), active(true), timer(3.0f), range(BOMB_RANGE) {
    currentFrame = 0;
    framesCounter = 0;
    framesSpeed = 6;
}

Bomb::~Bomb() {
    
}

float Bomb::Update(float deltaTime) {
    if (active) {
        timer -= deltaTime;

        framesCounter++;
        if (framesCounter >= (60 / framesSpeed)) {
            framesCounter = 0;
            currentFrame++;

            if (currentFrame > 2) {
                currentFrame = 0;
            }
        }
    }
    return timer;
}
void Bomb::Explode() {

    Vector2 v = { x,y };
    blasts.push_back(Blast(v));
    
}
void Bomb::Draw() const {
    
    if (active) {
        Vector2 v = { x, y };
        Rectangle source = { SPRITE_SIZE, 0, SPRITE_SIZE, SPRITE_SIZE };
        Rectangle dest = { (x ) , (y) , SPRITE_SIZE * 6.3f , SPRITE_SIZE * 6.3f};
        Vector2 v2 = { 1, 1 };

        source.x = SPRITE_SIZE * currentFrame;

        DrawTexturePro(texture, source, dest, v2, 0, WHITE);
        
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