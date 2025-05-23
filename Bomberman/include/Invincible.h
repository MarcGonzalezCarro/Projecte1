#pragma once
#ifndef INVINCIBLE_H
#define INVINCIBLE_H

#include "PowerUp.h"
#include <chrono>

class Invincible : public PowerUp {
public:
    Invincible(float x, float y);

    void Effect() override;

};

#endif // INVINCIBLE_H



