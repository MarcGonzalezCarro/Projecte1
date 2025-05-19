#include "Blast.h"
#include "raylib.h"
#include "Constants.h"

Blast::Blast(Vector2 pos) {
    position = pos;
    timer = 1.0f;  // Explosión dura 0.5 segundos
    active = true;
    reverseAnim = false;
    range = 1;

    currentFrame = 0;
    framesCounter = 0;
    framesSpeed = 6;

    fromBoss = false;

    // Expansión de la explosión
    affectedTiles.push_back(position);
    
    
}
void Blast::UnloadT() {
    
}
void Blast::Update(float deltaTime) {
     if (!active) return;

    timer -= deltaTime;

    framesCounter++;
    if (framesCounter >= (60 / framesSpeed)) {
        framesCounter = 0;
        if (!reverseAnim) {
            currentFrame++;
            if (currentFrame > 3) {
                reverseAnim = true;
                currentFrame--;
            }
        }
        else {
            currentFrame--;
            if (currentFrame <= 0) {
                reverseAnim = false;
            }
        }
    }

    if (timer <= 0) {
        active = false;
    }
    //printf("%d xdd\n", currentFrame);
}

void Blast::Draw() {
    if (!active) return;

    for (auto& tile : affectedTiles) {
        Vector2 v = {position.x, position.y};
        Rectangle source = { 0, 0, SPRITE_SIZE, SPRITE_SIZE };
        Rectangle dest = { (position.x) , (position.y) , SPRITE_SIZE * 6.3f , SPRITE_SIZE * 6.3f  };
        Vector2 v2 = { 1, 1 };
        
        if (direction == 0) {
            if (!finalB) {
                source.x = SPRITE_SIZE * 4 + 144 * currentFrame;
            }
            else {
                source.x = SPRITE_SIZE * 3 + 144 * currentFrame;
            }
        }
        if (direction == 1) {
            if (!finalB) {
                source.x = SPRITE_SIZE * 7 + 144 * currentFrame;
            }
            else {
                source.x = SPRITE_SIZE * 8 + 144 * currentFrame;
            }
        }
        if (direction == 2) {
            if (!finalB) {
                source.x = SPRITE_SIZE * 2 + 144 * currentFrame;
            }
            else {
                source.x = SPRITE_SIZE * 1 + 144 * currentFrame;
            }
        }
        if (direction == 3) {
            if (!finalB) {
                source.x = SPRITE_SIZE * 5 + 144 * currentFrame;
            }
            else {
                source.x = SPRITE_SIZE * 6 + 144 * currentFrame;
            }
        }
        if (direction == 4) {
            
            source.x = 0 + 144 * currentFrame;
            
        }
        DrawTexturePro(texture, source, dest, v2, 0, WHITE);
        
    }
}

void Blast::DrawHitbox() {
    if (!active) return;
    DrawRectangleLines(position.x, position.y, CELL_SIZE, CELL_SIZE, WHITE);
    
}

void Blast::SetTexture(Texture2D tex) {
    texture = tex;
}
void Blast::SetDirection(int dir) {
    direction = dir;
}
void Blast::SetFinal(bool status) {
    finalB = status;
}

void Blast::SetFromBoss(bool status)
{
    fromBoss = status;
}
