#ifndef ENEMY_H
#define ENEMY_H

#include "raylib.h"
#include "Wall.h"
#include "SoftBlock.h"
#include <vector>

class Enemy {
public:
    Enemy(Vector2 startPosition); // Constructor
    virtual ~Enemy(); // Destructor virtual para herencia

    virtual void Update(float deltaTime, const std::vector<Wall>& walls, const std::vector<SoftBlock>& softBlocks); // Actualizaci�n del enemigo (movimientos, l�gica)
    virtual void Draw() const;         // Dibuja el enemigo en pantalla

    Vector2 GetPosition() const; // Obtener la posici�n actual
    Vector2 GetDirection() const;
    void SetPosition(Vector2 newPosition); // Establecer posici�n

    void SetRandomDirection();
    void SetTexture(Texture2D newTexture);
    virtual bool CheckWallCollision(Vector2 nextPosition, const std::vector<Wall>& walls, const std::vector<SoftBlock>& softBlocks);

protected:
    Vector2 position; // Posici�n del enemigo
    float speed;      // Velocidad de movimiento del enemigo
    float distance;
    float framecounter;
    Texture2D texture;
    Vector2 direction;
    
};

#endif // ENEMY_H
