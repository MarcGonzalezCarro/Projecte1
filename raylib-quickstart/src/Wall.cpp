#include "Wall.h"
#include "Constants.h"

Wall::Wall(float x, float y) {
    position = { x, y, (float)CELL_SIZE, (float)CELL_SIZE };
    color = WHITE;
}

void Wall::Draw() {
    Rectangle camera = {position.x + CAMERA_OFFSET_X,position.y + CAMERA_OFFSET_Y,CELL_SIZE,CELL_SIZE};
    DrawRectangleRec(camera, color);
}

Rectangle Wall::GetBound() const
{
    return Rectangle(position);
}

