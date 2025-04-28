#ifndef MECHABLASTX_H
#define MECHABLASTX_H

#include "BossBase.h"
#include "raylib.h"
#include "Wall.h"
#include "SoftBlock.h"
#include <vector>

class MechaBlastX : public Boss{
public:
    MechaBlastX(Vector2 startPosition); // Constructor
    int id;
    virtual ~MechaBlastX(); // Destructor virtual para herencia

protected:
    
};

#endif // ENEMY_H
