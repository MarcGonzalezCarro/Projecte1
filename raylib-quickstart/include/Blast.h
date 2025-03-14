#ifndef BLAST_H
#define BLAST_H

#include "raylib.h"
#include <vector>

class Blast {
public:
    Vector2 position;   // Posición de la explosión en la cuadrícula
    float timer;        // Tiempo antes de desaparecer
    int range;          // Cuántos bloques se expande la explosión
    bool active;        // Si la explosión sigue activa
    std::vector<Vector2> affectedTiles; // Lista de celdas afectadas
    Texture2D texture;

    Blast(Vector2 pos);
    void UnloadT();

    void Update(float deltaTime);
    void Draw();
};


#endif
