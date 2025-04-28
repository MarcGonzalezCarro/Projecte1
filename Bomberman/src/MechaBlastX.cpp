#include "MechaBlastX.h"
#include "BossBase.h"
#include <iostream>
#include "raylib.h"
#include "Constants.h"

MechaBlastX::MechaBlastX(Vector2 startPosition)
    : Boss(startPosition) 
{

    id = 0;
    
}

MechaBlastX::~MechaBlastX()
{
}

void MechaBlastX::Update(float deltaTime, const std::vector<Wall>& walls, const std::vector<SoftBlock>& softBlocks)
{

}

void MechaBlastX::Draw() const
{

}


