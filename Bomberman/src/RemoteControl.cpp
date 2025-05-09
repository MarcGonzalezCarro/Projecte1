#include "RemoteControl.h"
#include "Constants.h"

RemoteControl::RemoteControl(float x, float y) : PowerUp(x, y) {
    id = 4;
}

void RemoteControl::Effect(){
    MAX_BOMBS++;
}
