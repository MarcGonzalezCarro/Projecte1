#ifndef BLAST_H
#define BLAST_H

#include "raylib.h"
#include <vector>

class Blast {
public:
    Vector2 position;   // Posici�n de la explosi�n en la cuadr�cula
    float timer;        // Tiempo antes de desaparecer
    int range;          // Cu�ntos bloques se expande la explosi�n
    bool active;        // Si la explosi�n sigue activa
    std::vector<Vector2> affectedTiles; // Lista de celdas afectadas
    Texture2D texture;

    Blast(Vector2 pos);
    void UnloadT();

    void Update(float deltaTime);
    void Draw();
};


#endif
