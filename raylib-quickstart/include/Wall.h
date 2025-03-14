#pragma once
#ifndef WALL_H
#define WALL_H

#include "raylib.h"

class Wall {
private:
    Rectangle position;
    Color color;

public:
    Wall(float x, float y);
    void Draw();
    Rectangle GetBound() const;
    
};

#endif
