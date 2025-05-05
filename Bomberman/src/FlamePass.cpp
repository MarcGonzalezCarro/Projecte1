#include "BombUp.h"
#include "Constants.h"

BombUp::BombUp(float x, float y) : PowerUp(x, y) {
    id = 0;
}

void BombUp::Effect(){
    MAX_BOMBS++;
}
