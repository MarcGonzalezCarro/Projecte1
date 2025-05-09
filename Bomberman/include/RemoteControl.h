#pragma once
#ifndef REMOTECONTROL_H
#define REMOTECONTROL_H

#include "PowerUp.h"

class RemoteControl : public PowerUp {
public:
    RemoteControl(float x, float y);
    void Effect() override;
};

#endif // REMOTECONTROL_H

