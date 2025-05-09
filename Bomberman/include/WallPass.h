#pragma once
#ifndef WALLPASS_H
#define WALLPASS_H

#include "PowerUp.h"

class WallPass : public PowerUp {
public:
    WallPass(float x, float y);
    void Effect() override;
};

#endif // WALLPASS_H

