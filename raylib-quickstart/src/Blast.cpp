#include "Blast.h"
#include "raylib.h"
#include "Constants.h"

Blast::Blast(Vector2 pos) {
    position = pos;
    timer = 0.5f;  // Explosión dura 0.5 segundos
    active = true;
    range = 1;

    // Expansión de la explosión
    affectedTiles.push_back(position);
    
    
}
void Blast::UnloadT() {
    
}
void Blast::Update(float deltaTime) {
    if (!active) return;

    timer -= deltaTime;
    if (timer <= 0) {
        active = false;
    }
}

void Blast::Draw() {
    if (!active) return;

    for (auto& tile : affectedTiles) {
        Vector2 v = {position.x + CAMERA_OFFSET_X, position.y + CAMERA_OFFSET_Y};
        DrawTextureEx(texture, v, 0, 6, WHITE);
        
    }
}

void Blast::SetTexture(Texture2D tex) {
    texture = tex;
}