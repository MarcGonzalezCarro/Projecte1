#pragma once
#ifndef POWERUP_H
#define POWERUP_H

#include "raylib.h"

class PowerUp {
private:
    Rectangle position;
    Color color;
    Texture2D texture;
    

public:
    PowerUp(float x, float y);
    int id;
    void Draw();
    void DrawHitbox();
    Rectangle GetBound() const;
    void SetTexture(Texture2D text);
    virtual void Effect();
    virtual void Update() {}
};

#endif
