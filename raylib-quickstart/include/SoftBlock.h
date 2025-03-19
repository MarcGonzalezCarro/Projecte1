#pragma once
#ifndef SOFTBLOCK_H
#define SOFTBLOCK_H

#include "raylib.h"
#include "vector"

class SoftBlock {
private:
    Rectangle position;
    Color color;
    bool isDestroyed;
    bool isActive;
    Texture2D texture;

    int framesCounter;
    int framesSpeed;
    int currentFrame;

public:
    SoftBlock(float x, float y);
    void Destroy();
    void Draw();
    bool IsDestroyed() const;
    bool IsActive() const;
    void Update();
    Rectangle GetBound() const;
    void SetTexture(Texture2D text);
    
};

#endif
