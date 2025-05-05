#ifndef MECHABLASTX_H
#define MECHABLASTX_H

#include "BossBase.h"
#include "raylib.h"
#include "Wall.h"
#include "SoftBlock.h"
#include <vector>
#include "Blast.h"

class MechaBlastX : public Boss{
public:
    MechaBlastX(Vector2 startPosition); // Constructor
    int id;
    std::vector<Blast>& GetBlasts() { return blasts; }
    virtual ~MechaBlastX(); // Destructor virtual para herencia
    void Update(float deltaTime, const std::vector<Wall>& walls, const std::vector<SoftBlock>& softBlocks) override;
    void Draw() const override;

    void Phase1Attack(Vector2 pos);
protected:
    std::vector<Blast> blasts;
};

#endif // ENEMY_H
