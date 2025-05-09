#include "WallPass.h"
#include "Constants.h"

WallPass::WallPass(float x, float y) : PowerUp(x, y) {
    id = 3;
}

void WallPass::Effect(){
    PUWP = true;
}
