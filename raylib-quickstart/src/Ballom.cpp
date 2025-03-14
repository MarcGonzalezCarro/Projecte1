#include "Ballom.h"
#include <cstdlib> // Para generar n�meros aleatorios
#include <ctime>   // Semilla de aleatoriedad
#include "raylib.h"
#include "Constants.h"

Ballom::Ballom(Vector2 startPosition)
    : Enemy(startPosition), direction({ 1, 0 }), dropBombProbability(0.1f) // Inicializamos con direcci�n y probabilidad
{
    std::srand(static_cast<unsigned int>(std::time(0))); // Semilla aleatoria
}

Ballom::~Ballom() {}


void Ballom::Draw() const {
    DrawRectangle(position.x + CAMERA_OFFSET_X, position.y + CAMERA_OFFSET_Y, CELL_SIZE, CELL_SIZE, GREEN);
    //std::cout << "Renderizando enemigo m�vil en posici�n: (" << position.x << ", " << position.y << ")" << std::endl;
}

void Ballom::DropBomb() {
    //std::cout << "�El enemigo ha dejado una bomba en posici�n: (" << position.x << ", " << position.y << ")!" << std::endl;
}
