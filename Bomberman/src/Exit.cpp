#include "Exit.h"
#include "Constants.h"

Exit::Exit(float x, float y) {
    position = { x, y, (float)CELL_SIZE, (float)CELL_SIZE };
    color = WHITE;
}

void Exit::Draw() {
    Rectangle camera = {position.x,position.y,CELL_SIZE,CELL_SIZE};
    Rectangle source = { 0, 0, SPRITE_SIZE, SPRITE_SIZE };
    Rectangle dest = { (position.x) , (position.y) , SPRITE_SIZE * 6.3f , SPRITE_SIZE * 6.3f  };
    Vector2 v2 = { 1, 1 };

    DrawTexturePro(texture, source, dest, v2, 0, WHITE);
}

void Exit::DrawHitbox() {
    DrawRectangleLines(position.x, position.y, CELL_SIZE, CELL_SIZE, GREEN);
}

Rectangle Exit::GetBound() const
{
    return Rectangle(position);
}

void Exit::SetTexture(Texture2D text) {
    texture = text;
}
