#include "Wall.h"
#include "Constants.h"

Wall::Wall(float x, float y) {
    position = { x, y, (float)CELL_SIZE, (float)CELL_SIZE };
    color = WHITE;
}

void Wall::Draw() {
    Rectangle camera = {position.x + CAMERA_OFFSET_X,position.y + CAMERA_OFFSET_Y,CELL_SIZE,CELL_SIZE};
    Rectangle source = { 0, 0, SPRITE_SIZE, SPRITE_SIZE };
    Rectangle dest = { position.x + CAMERA_OFFSET_X, position.y + CAMERA_OFFSET_Y, SPRITE_SIZE * 6.3f, SPRITE_SIZE * 6.3f };
    Vector2 v2 = { 1, 1 };

    DrawTexturePro(texture, source, dest, v2, 0, WHITE);
}

Rectangle Wall::GetBound() const
{
    return Rectangle(position);
}

void Wall::SetTexture(Texture2D text) {
    texture = text;
}
