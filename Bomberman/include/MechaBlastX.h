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
    void Update(float deltaTime, const std::vector<Wall>& walls, const std::vector<SoftBlock>& softBlocks) override;
    void Draw() const override;
protected:
    
};

#endif // ENEMY_H
