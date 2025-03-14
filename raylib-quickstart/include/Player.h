#ifndef PLAYER_H
#define PLAYER_H

#include "raylib.h"

class Player {
private:
    int x, y;           // Posición del jugador
    int life;           // Cantidad de vidas
    Texture2D texture;  // Textura del jugador

public:
    Player(int startX, int startY);
    ~Player();

    void Move(int dx, int dy);
    void Draw() const;
    
    void SetPosition(int newX, int newY);
    void SetX(int newX);
    void SetY(int newY);
    Rectangle GetBounds() const;

    int GetX() const;
    int GetY() const;
    int GetLife() const;
    void DecreaseLife();
};

#endif
