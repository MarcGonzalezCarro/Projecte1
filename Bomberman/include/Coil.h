#ifndef COIL_H
#define COIL_H

#include "raylib.h"
#include "SoftBlock.h"
#include <vector>

class Coil {
public:
    Coil(Vector2 startPosition); // Constructor
    int id;
    virtual ~Coil(); // Destructor virtual para herencia

    void Update(float deltaTime); // Actualización del enemigo (movimientos, lógica)
    void Draw() const;         // Dibuja el enemigo en pantalla

    void SetPosition(Vector2 newPosition); // Establecer posición

    void SetTexture(Texture2D newTexture);

    bool IsActive();
    bool IsDead();
    void Die();

protected:
    Vector2 position; // Posición del enemigo
    
    
    float framecounter;
    Texture2D texture;

    
    int currentFrame;
    int animFramesCounter;
    int framesSpeed;
 

    bool isDead;
    bool isActive;
};

#endif
