#pragma once
#ifndef FLAMEPASS_H
#define FLAMEPASS_H

#include "PowerUp.h"

class FlamePass : public PowerUp {
public:
    FlamePass(float x, float y);
    void Effect() override;
};

#endif // FLAMEPASS_H

