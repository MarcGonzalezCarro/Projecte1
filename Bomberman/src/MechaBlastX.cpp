#include "MechaBlastX.h"
#include "BossBase.h"
#include <iostream>
#include "raylib.h"
#include "Constants.h"

MechaBlastX::MechaBlastX(Vector2 startPosition)
    : Boss(startPosition) 
{
    isAttacking = false;
    id = 0;
    isDead = false;
    framecounter = 120;
    currentFrame = 0;
    animFramesCounter = 0;
    direction = { 1, 0 };
    framesSpeed = 12;
}

MechaBlastX::~MechaBlastX()
{
}

void MechaBlastX::Update(float deltaTime, const std::vector<Wall>& walls, const std::vector<SoftBlock>& softBlocks)
{
    if (isAttacking == true) {
        if (!isDead) {
            framecounter -= deltaTime;

            if (framecounter <= 0) {
                framecounter = 120;  // Resetear en segundos, no en frames
            }
        }
        animFramesCounter++;
        if (animFramesCounter >= (60 / framesSpeed)) {
            animFramesCounter = 0;
            currentFrame++;
        }
        if (!isDead) {
            if (currentFrame > 27) {
                currentFrame = 0;
                isAttacking = false;
            }
        }
    }

}

void MechaBlastX::Draw() const
{
   
    // Dibujar el enemigo 
    Vector2 v = { position.x, position.y };
    Rectangle source = { 0, 0, 78, 46 };
    Rectangle dest = { (position.x), (position.y), 78 * 6.3f , 46 * 6.3f };
    Vector2 v2 = { 1, 1 };
    if (isDead == false) {
        if (direction.x == 1 && direction.y == 0) {
            source.x = 78 * currentFrame;
        }
        if (direction.x == -1 && direction.y == 0) {
            source.x = 78 * 2 + 78 * currentFrame;
        }
        if (direction.x == 0 && direction.y == 1) {
            source.x = 78 * currentFrame;
        }
        if (direction.x == 0 && direction.y == -1) {
            source.x = 78 * 2 + SPRITE_SIZE * currentFrame;
        }
    }
    else {
        source.x = 78 * 5 + 78 * currentFrame;
    }
    DrawTexturePro(texture, source, dest, v2, 0, WHITE);

}



void MechaBlastX::GenerateBlasts(Vector2 playerPosition)
{
    // Generar los blasts alrededor de la zona de 3x3
    for (int dx = -1; dx <= 1; ++dx)
    {
        for (int dy = -1; dy <= 1; ++dy)
        {
            Vector2 blastPos = { playerPosition.x + dx * CELL_SIZE, playerPosition.y + dy * CELL_SIZE };
            // Crear y añadir el blast a la lista de blasts
            blasts->push_back(Blast(blastPos));
            printf("YEEE");
        }
    }
}

void MechaBlastX::Phase1Attack(Vector2 pos)
{
    // Zona roja alrededor del jugador en un área 3x3
    float zoneSize = 3.0f * CELL_SIZE; // 3x3, cada celda del sprite tiene 78 px de ancho
    Vector2 topLeft = { pos.x - zoneSize / 2.0f, pos.y - zoneSize / 2.0f };
    Vector2 bottomRight = { pos.x + zoneSize / 2.0f, pos.y + zoneSize / 2.0f };

    // Dibujar la zona roja (visualizarla)
    if (!isDead) {
        DrawRectangleRec({ topLeft.x, topLeft.y, zoneSize, zoneSize }, RED);
    }

    // Aquí gestionamos el temporizador de espera antes de generar los blasts
    attackDelayCounter -= GetFrameTime(); // Reduce el contador de tiempo

    if (attackDelayCounter <= 0.0f) {
        // Ya pasó el retraso, ahora generamos los blasts
        GenerateBlasts(pos);
        attackDelayCounter = attackDelayDuration; // Reiniciar el temporizador
    }
}




