#ifndef PLAYER_H
#define PLAYER_H

#include "raylib.h"

class Player {
private:
    int x, y;           // Posición del jugador
    int life;           // Cantidad de vidas
    int currentFrame;
    int framesCounter;
    int framesSpeed;
    Texture2D texture;  // Textura del jugador
    Vector2 prevdirection;
    Vector2 newdirection;

public:
    Player(int startX, int startY);
    ~Player();

    void Move(int dx, int dy, Vector2 dir);
    void Update();
    void Draw() const;
    
    void SetPosition(int newX, int newY);
    void SetX(int newX);
    void SetY(int newY);
    void SetDirection(Vector2 dir);
    void SetTexture(Texture2D texture);
    Rectangle GetBounds() const;

    int GetX() const;
    int GetY() const;
    int GetLife() const;
    void DecreaseLife();
    
};

#endif
