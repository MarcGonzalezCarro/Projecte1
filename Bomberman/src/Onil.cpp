#include "Onil.h"
#include <cstdlib> 
#include <ctime>   
#include "raylib.h"
#include "Constants.h"

Onil::Onil(Vector2 startPosition)
    : Enemy(startPosition), direction({ 1, 0 }), dropBombProbability(0.1f)
{
    id = 1;
    score = 200;

}

Onil::~Onil() {}

