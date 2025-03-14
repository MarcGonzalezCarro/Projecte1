#ifndef ENEMY_H
#define ENEMY_H

#include "raylib.h"
#include "Wall.h"
#include <vector>

class Enemy {
public:
    Enemy(Vector2 startPosition); // Constructor
    virtual ~Enemy(); // Destructor virtual para herencia

    virtual void Update(float deltaTime, const std::vector<Wall>& walls); // Actualizaci�n del enemigo (movimientos, l�gica)
    virtual void Draw() const;         // Dibuja el enemigo en pantalla

    Vector2 GetPosition() const; // Obtener la posici�n actual
    void SetPosition(Vector2 newPosition); // Establecer posici�n

    void SetRandomDirection();

protected:
    Vector2 position; // Posici�n del enemigo
    float speed;      // Velocidad de movimiento del enemigo
    float distance;
    float framecounter;
    Vector2 direction;
    bool CheckWallCollision(Vector2 nextPosition, const std::vector<Wall>& walls);
};

#endif // ENEMY_H
