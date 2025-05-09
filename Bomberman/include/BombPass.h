#pragma once
#ifndef BOMBPASS_H
#define BOMBPASS_H

#include "PowerUp.h"

class BombPass : public PowerUp {
public:
    BombPass(float x, float y);
    void Effect() override;
};

#endif // BOMBPASS_H

