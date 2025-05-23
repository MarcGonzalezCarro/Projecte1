#include "BombPass.h"
#include "Constants.h"

BombPass::BombPass(float x, float y) : PowerUp(x, y) {
    id = 5;
}

void BombPass::Effect(){
    PUBP = true;
}
