#include "FlamePass.h"
#include "Constants.h"

FlamePass::FlamePass(float x, float y) : PowerUp(x, y) {
    id = 6;
}

void FlamePass::Effect(){
    MAX_BOMBS++;
}
