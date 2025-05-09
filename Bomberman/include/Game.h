#ifndef GAME_H
#define GAME_H

#include "raylib.h"
#include "Player.h"
#include "Bomb.h"
#include "Wall.h"
#include "SoftBlock.h"
#include "Blast.h"
#include <vector>

class Game {
private:
    Player player;
    Player player2;
    std::vector<Wall> walls;
    std::vector<SoftBlock> softBlocks;
    std::vector<Bomb*> bombs;
    
    bool onBomb;
    

public:
    Game();
    void Run();
    void Update();
    void Draw();
    void AddWalls();
    void AddBomb(float x, float y);
    void AddEnemy(Vector2 pos);
    void AddBoss(Vector2 pos);
    void AddBlasts(Bomb bomb);
    void ExplodeBombs();
    void ResetStage();
    int CheckCollisions(Rectangle rec);
    int CheckPlayerCollisions(Rectangle rec);
    bool IsBlastBlocked(Vector2 position);
    bool CheckBlastDamage(Vector2 pos);
    void GameOver();
    void NextLevel();
    void ShowStageScreen(const char* stageText);
    void CheckPowerUPCollision(Rectangle rec);
    void CheckExitCollision();
    bool CheckEnemyCollision(float x, float y);
    void BossLevel();
};

extern std::vector<Blast> blasts;

#endif
