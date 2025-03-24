#include "FireUp.h"
#include "Constants.h"

FireUp::FireUp(float x, float y) : PowerUp(x, y) {
    id = 1;
}

void FireUp::Effect() {
    BOMB_RANGE++;
}
