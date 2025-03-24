#pragma once
#ifndef FIREUP_H
#define FIREUP_H

#include "PowerUp.h"

class FireUp : public PowerUp {
public:
    FireUp(float x, float y);
    void Effect() override;
};

#endif // FIREUP_H

