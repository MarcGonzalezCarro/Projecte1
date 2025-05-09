#pragma once
#ifndef INVINCIBLE_H
#define INVINCIBLE_H

#include "PowerUp.h"
#include <chrono>

class Invincible : public PowerUp {
public:
    Invincible(float x, float y);

    void Effect() override;
    void Update() override; // Método nuevo para controlar el tiempo
private:
    std::chrono::time_point<std::chrono::steady_clock> startTime;
    bool active = false;
};

#endif // INVINCIBLE_H



