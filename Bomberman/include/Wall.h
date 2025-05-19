#pragma once
#ifndef WALL_H
#define WALL_H

#include "raylib.h"

class Wall {
private:
    Rectangle position;
    Color color;
    Texture2D texture;

public:
    Wall(float x, float y);
    void Draw();
    void DrawHitbox();
    Rectangle GetBound() const;
    void SetTexture(Texture2D text);
};

#endif
