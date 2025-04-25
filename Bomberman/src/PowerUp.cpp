#include "PowerUp.h"
#include "Constants.h"

PowerUp::PowerUp(float x, float y) {
    position = { x, y, (float)CELL_SIZE, (float)CELL_SIZE };
    color = WHITE;
}

void PowerUp::Draw() {
    Rectangle camera = {position.x,position.y,CELL_SIZE,CELL_SIZE};
    Rectangle source = { SPRITE_SIZE * id, 0, SPRITE_SIZE, SPRITE_SIZE };
    Rectangle dest = { (position.x ), (position.y ) , SPRITE_SIZE * 6.3f , SPRITE_SIZE * 6.3f  };
    Vector2 v2 = { 1, 1 };

    DrawTexturePro(texture, source, dest, v2, 0, WHITE);
}

Rectangle PowerUp::GetBound() const
{
    return Rectangle(position);
}

void PowerUp::SetTexture(Texture2D text) {
    texture = text;
}

void PowerUp::Effect() {

}
