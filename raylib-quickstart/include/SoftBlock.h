#pragma once
#ifndef SOFTBLOCK_H
#define SOFTBLOCK_H

#include "raylib.h"

class SoftBlock {
private:
    Rectangle position;
    Color color;
    bool isDestroyed;

public:
    SoftBlock(float x, float y);
    void Destroy();
    void Draw();
    bool IsDestroyed() const;
    Rectangle GetBound() const;
    
};

#endif
