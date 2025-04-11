#pragma once
#ifndef SPEEDUP_H
#define SPEEDUP_H

#include "PowerUp.h"

class SpeedUp : public PowerUp {
public:
    SpeedUp(float x, float y);
    void Effect() override;
};

#endif // SPEEDUP_H
