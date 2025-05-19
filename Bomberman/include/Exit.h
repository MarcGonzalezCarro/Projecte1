#pragma once
#ifndef EXIT_H
#define EXIT_H

#include "raylib.h"

class Exit {
private:
    Rectangle position;
    Color color;
    Texture2D texture;

public:
    Exit(float x, float y);
    void Draw();
    void DrawHitbox();
    Rectangle GetBound() const;
    void SetTexture(Texture2D text);
};

#endif
