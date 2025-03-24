#include "SpeedUp.h"
#include "Constants.h"

SpeedUp::SpeedUp(float x, float y) : PowerUp(x, y) {
    id = 3;
}

void SpeedUp::Effect() {
    PLAYER_SPEED++;
}
