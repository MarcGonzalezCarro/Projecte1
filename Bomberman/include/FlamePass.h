#pragma once
#ifndef BOMBUP_H
#define BOMBUP_H

#include "PowerUp.h"

class BombUp : public PowerUp {
public:
    BombUp(float x, float y);
    void Effect() override;
};

#endif // BOMBUP_H

