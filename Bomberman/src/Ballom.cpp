#include "Ballom.h"
#include <cstdlib> 
#include <ctime>   
#include "raylib.h"
#include "Constants.h"

Ballom::Ballom(Vector2 startPosition)
    : Enemy(startPosition), direction({ 1, 0 }), dropBombProbability(0.1f) 
{
    id = 0;
    score = 100;
   
}

Ballom::~Ballom() {}