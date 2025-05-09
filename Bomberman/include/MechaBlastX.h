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
    int attackDelayCounter;
    int attackDelayDuration;
    //std::vector<Blast>&GetBlasts() { return blasts; }
    virtual ~MechaBlastX(); // Destructor virtual para herencia
    void Update(float deltaTime, const std::vector<Wall>& walls, const std::vector<SoftBlock>& softBlocks) override;
    void Draw() const override;
    void GenerateBlasts(Vector2 playerPosition);
    void Phase1Attack(Vector2 pos) override;
    
protected:
    
    float attackDelayCounter = 3.0f; // Retraso de 3 segundos para la fase de ataque
    float attackDelayDuration = 3.0f; // Duración del retraso
    std::vector<Blast>* blasts; // Puntero al vector de blasts compartido
};

#endif // ENEMY_H
