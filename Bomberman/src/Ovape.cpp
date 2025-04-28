#include "Ovape.h"
#include <cstdlib> 
#include <ctime>   
#include "raylib.h"
#include "Constants.h"

Ovape::Ovape(Vector2 startPosition)
    : Enemy(startPosition), direction({ 1, 0 }), dropBombProbability(0.1f)
{
    id = 5;
    score = 2000;

}

Ovape::~Ovape() {}