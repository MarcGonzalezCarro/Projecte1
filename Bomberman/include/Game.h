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
    void AddBlasts1(Vector2 pos);
    void AddCoil(Vector2 pos);
    void AddEnergyShield(Vector2 pos);
    void ExplodeBombs();
    void ResetStage();
    int CheckCollisions(Rectangle rec);
    int CheckPlayerCollisions(Rectangle rec);
    bool IsBlastBlocked(Vector2 position);
    bool CheckBlastDamage(Vector2 pos);
    bool CheckPlayerBlastDamage(Vector2 pos);
    void GameOver();
    void NextLevel();
    void ShowStageScreen(const char* stageText);
    void CheckPowerUPCollision(Rectangle rec);
    void CheckExitCollision();
    bool CheckEnemyCollision(float x, float y);
    void BossLevel();
    void PrepareBossAttack1();
    void PrepareBossAttack2();
    void PrepareBossAttack3();
    void BossAttack1();
    void CreateCircle(int centerX, int centerY, int radius);
    void StartBlastPropagation(int centerX, int centerY, int initialRadius);
    void BossAttack2();
    void BossCoroutine(float time);
};

extern std::vector<Blast> blasts;

#endif
