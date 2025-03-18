#ifndef BOMB_H
#define BOMB_H

#include "raylib.h"

class Bomb {
private:
    int x, y;             // Posición de la bomba
    bool active;          // Estado de la bomba (activa/inactiva)
    float timer;          // Temporizador para explosión
    int range;
    Texture2D texture;    // Textura de la bomba

public:
     
    Bomb(int startX, int startY);
    ~Bomb();

    float Update(float deltaTime);
    void Draw() const;
    bool HasExploded() const;
    void Explode();
    Rectangle GetBounds() const;

    void Activate(int newX, int newY);
    bool IsActive() const;
   
    void SetTexture(Texture2D texture);

    int GetX() const;
    int GetY() const;
    int GetRange() const;
};

#endif
