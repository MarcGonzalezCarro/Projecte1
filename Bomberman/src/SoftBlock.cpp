#include "SoftBlock.h"
#include "Constants.h"

SoftBlock::SoftBlock(float x, float y) {
    position = { x, y, (float)CELL_SIZE, (float)CELL_SIZE };
    color = GRAY;
    isDestroyed = false;
    isActive = true;
    currentFrame = 0;
    framesCounter = 0;
    framesSpeed = 22;
}

void SoftBlock::Destroy() {
    isDestroyed = true;
}

void SoftBlock::Draw() {
    if (isActive) {
        Rectangle rec = { position.x , position.y, position.width, position.height };
        Rectangle source = { SPRITE_SIZE + SPRITE_SIZE * currentFrame, 0, SPRITE_SIZE, SPRITE_SIZE };
        Rectangle dest = { (position.x ), (position.y) , SPRITE_SIZE * 6.3f , SPRITE_SIZE * 6.3f };
        Vector2 v2 = { 1, 1 };

        DrawTexturePro(texture, source, dest, v2, 0, WHITE);
    }
}

void SoftBlock::DrawHitbox() {
    if (isActive) {
        DrawRectangleLines(position.x,position.y, CELL_SIZE, CELL_SIZE, GREEN);
    }
}

void SoftBlock::Update() {

    framesCounter++;
    if (framesCounter >= (60 / framesSpeed)) {
        framesCounter = 0;
        currentFrame++;

        if (currentFrame > 6) {
            isActive = false;
        }
    }

}

bool SoftBlock::IsDestroyed() const {
    return isDestroyed;
}

bool SoftBlock::IsActive() const{
    return isActive;
}
Rectangle SoftBlock::GetBound() const
{
    return Rectangle(position);
}

void SoftBlock::SetTexture(Texture2D text) {
    texture = text;
}
