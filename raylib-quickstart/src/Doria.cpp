#include "Doria.h"
#include <cstdlib> 
#include <ctime>   
#include "raylib.h"
#include "Constants.h"

Doria::Doria(Vector2 startPosition)
    : Enemy(startPosition), direction({ 1, 0 }), dropBombProbability(0.1f)
{



}

Doria::~Doria() {}


void Doria::Draw() const {
    //DrawRectangle(position.x + CAMERA_OFFSET_X, position.y + CAMERA_OFFSET_Y, CELL_SIZE, CELL_SIZE, GREEN);


}

void Doria::DropBomb() {

}
