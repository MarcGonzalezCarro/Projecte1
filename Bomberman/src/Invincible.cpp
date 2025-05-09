#include "Invincible.h"
#include "Constants.h"

Invincible::Invincible(float x, float y) : PowerUp(x, y) {
    id = 7;
}

void Invincible::Effect() {
    INVINCIBLE = true;
    active = true;
    startTime = std::chrono::steady_clock::now();
}

void Invincible::Update(){
    printf("aaa");
    if (active) {
        auto now = std::chrono::steady_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::seconds>(now - startTime).count();

        if (duration >= 5) {
            INVINCIBLE = false;
            active = false;
        }
    }
}