#include "SoftBlock.h"
#include "Constants.h"

SoftBlock::SoftBlock(float x, float y) {
    position = { x, y, (float)CELL_SIZE, (float)CELL_SIZE };
    color = GRAY;
    isDestroyed = false;
}

void SoftBlock::Destroy() {
    isDestroyed = true;
}

void SoftBlock::Draw() {
    if (!isDestroyed) {
        Rectangle rec = { position.x + CAMERA_OFFSET_X , position.y + CAMERA_OFFSET_Y, position.width, position.height };

        DrawRectangleRec(rec, color);
    }
}

bool SoftBlock::IsDestroyed() const {
    return isDestroyed;
}

Rectangle SoftBlock::GetBound() const
{
    return Rectangle(position);
}


