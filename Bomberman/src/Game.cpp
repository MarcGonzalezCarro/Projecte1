#include "Game.h"
#include "Constants.h"
#include <iostream>
#include "Blast.h"
#include "Ballom.h"
#include "Onil.h"
#include "Dahl.h"
#include "Minvo.h"
#include "Doria.h"
#include "Ovape.h"
#include "Pass.h"
#include "Pontan.h"
#include "Customizer.h"
#include "CustomizerPU.h"
#include "BossBase.h"
#include "MechaBlastX.h"
#include "PowerUp.h"
#include "BombUp.h"
#include "FireUp.h"
#include "WallPass.h"
#include "BombPass.h"
#include "FlamePass.h"
#include "Invincible.h"
#include "RemoteControl.h"
#include "SpeedUp.h"
#include "Bomb.h"
#include "Exit.h"
#include "Coil.h"
#include "EnergyShield.h"
#include "SaveGame.h"
#include "ResourceManager.h"
#include <sstream>
#include <string>
#include <algorithm>
#include <raymath.h>

#define INTERNAL_WIDTH 1920
#define INTERNAL_HEIGHT 1080

Customizer customizer;
CustomizerPU customizerPU;
ResourceManager resourceManager;
Texture2D initialScreen;
Texture2D arrowTexture;
Texture2D endScreen;

bool startScreen = true;
bool gameRunning = false;
bool inCredits = false;
bool inTop = false;
bool viewingTop = false;
bool isDead = false;

int menuChoice = 0;
int topScoreChoice = 0;
bool viewingScores = false;
int maxChoices = 4;

char playerName[6] = "";  // Buffer
int nameLength = 0;
int playerScore = 0;

Vector2 arrowPositions[] = {
    {565, 660}, 
    {820, 660},  
    {600, 720},
    {840, 720}
};

struct ScoreEnemies
{
    Texture2D textura;
    Vector2 pos;
    int id;
    int time;
};

std::vector<std::unique_ptr<ScoreEnemies>> scores;

int currentStage = 1;
bool extraEnemies = false;
bool noTimeLeft = false;

bool debbugerMode1 = false;
bool debbugerMode2 = false;
int totalEnemiesThisStage = 0;
int enemySelected = 0;
int powerUpSelected = 0;

bool explodeB = false;
int invincibleTimer = 0;

Camera2D camera1 = { 0 };
Camera2D camera2 = { 0 };

std::vector<ScoreEntry> entries = SaveGame::GetEntriesFromFile();
std::vector<Blast> blasts; 
std::vector<std::unique_ptr<PowerUp>> powerups;
std::vector<std::unique_ptr<Enemy>> enemies;
std::vector<Exit> exits;

std::unique_ptr<Boss> currentBoss;
std::vector<std::unique_ptr<Coil>> coils;
std::unique_ptr<EnergyShield> energyShield;

float bossAttackCooldown = 5.0f;
float zoneMarkCooldown = 2.0f;
bool attack1state = false;
float propagationTime;
float expansionSpeed = 2.0f;
int currentRadius = 3;
int radius = 3;
float maxRadius = 6;
int centerX, centerY;
bool propagationState;
int currentCoils = 3;
int currentPhase = 1;
std::vector<Rectangle> zoneMark;

std::vector<Vector2> emptyPositions;

Music menuSong;
Music gameSong;
Music specialGameSong;

int enemyCounter = 0;

bool playerWalking = false;
bool playerWalking2 = false;
bool isCoop = false;

double startTime = GetTime();
double targetTime = 200.0;
double elapsedTime;
double remainingTime = 200.0;

Color semiTransparentRed = Color{ 255, 0, 0, 80};

RenderTexture2D renderTarget;
bool resolutionChanged = false;

////////////////////////////
//FOR STATS
////////////////////////////
int enemiesKilled = 0;
int ballomKills = 0;
int onilKills = 0;
int dahlKills = 0;
int minvoKills = 0;
int doriaKills = 0;
int ovapeKills = 0;
int passKills = 0;
int pontanKills = 0;
int bombsPlanted = 0;
int softBlocksDestroyed = 0;
int powerUpsPicked = 0;
int fireUpCounter = 0;
int bombUpCounter = 0;
int speedUpCounter = 0;
int flamePassCounter = 0;
int bombPassCounter = 0;
int InvincibleCounter = 0;
int remoteControlCounter = 0;
int wallPassCounter = 0;
////////////////////////////

Game::Game() : player(INITIAL_PLAYER_X, INITIAL_PLAYER_Y), player2(INITIAL_PLAYER_X,INITIAL_PLAYER_Y), onBomb(false) {
    InitAudioDevice();
    resourceManager.LoadTextures();
    resourceManager.LoadMusic();
    resourceManager.LoadSounds();
    initialScreen = resourceManager.GetTexture(15);
    arrowTexture = resourceManager.GetTexture(16);
    endScreen = resourceManager.GetTexture(21);
    menuSong = resourceManager.GetMusic(0);
    gameSong = resourceManager.GetMusic(1);
    specialGameSong = resourceManager.GetMusic(2);
    customizer.LoadFromFile("resources/customization.txt");
    customizerPU.LoadFromFile("resources/customizationPU.txt");
    startTime = GetTime();
    targetTime = 200.0;
    
    renderTarget = LoadRenderTexture(INTERNAL_WIDTH, INTERNAL_HEIGHT);
    SetTextureFilter(renderTarget.texture, TEXTURE_FILTER_POINT);
}

void Game::Run() {
    
    player.SetTexture(resourceManager.GetTexture(0));
    player.SetDirection({ 1, 0 });
    player2.SetTexture(resourceManager.GetTexture(1));
    player2.SetDirection({ 1, 0 });

    AddBoss({1400,400});


    while (!WindowShouldClose()) {
        ///////////////////////////////////////////////////////
        //Menu
        ///////////////////////////////////////////////////////

        if (startScreen) {

            if (!IsMusicStreamPlaying(menuSong)) {
                StopMusicStream(gameSong);
                PlayMusicStream(menuSong);
                menuSong.looping = true;
            }
            else {
                UpdateMusicStream(menuSong);
            }

            if (IsKeyPressed(KEY_RIGHT)) {
                menuChoice++;
                if (menuChoice >= maxChoices) {
                    menuChoice = 0;
                }
            }
            if (IsKeyPressed(KEY_LEFT)) {
                menuChoice--;
                if (menuChoice < 0) {
                    menuChoice = maxChoices - 1;
                }
            }
            if (IsKeyPressed(KEY_ENTER)) { 

                switch (menuChoice)
                {
                case 0:
                    printf("Aqui seria el start");
                    startScreen = false;
                    ResetStage();
                    break;
                case 1:
                    printf("Aqui seria el continue");
                    startScreen = false;
                    isCoop = true;
                    ResetStage();
                    break;
                case 2:
                    startScreen = false;
                    inTop = true;
                    break;
                case 3:
                    startScreen = false;
                    inCredits = true;
                    break;
                default:
                    break;
                }
                
            }
        }
        else if (gameRunning) {
            if (!IsMusicStreamPlaying(gameSong) && !IsMusicStreamPlaying(specialGameSong)) {
                StopMusicStream(menuSong);
                if (powerUpsPicked == 0) {
                    PlayMusicStream(gameSong);
                    gameSong.looping = true;
                }
                else {
                    PlayMusicStream(specialGameSong);
                    specialGameSong.looping = true;
                }                
            }
            else if(IsMusicStreamPlaying(gameSong)){
                UpdateMusicStream(gameSong);
            }
            else if (IsMusicStreamPlaying(specialGameSong)) {
                UpdateMusicStream(specialGameSong);
            }
            Update();
        }
        else if (inCredits) {
            if (IsKeyPressed(KEY_ENTER)) {
                startScreen = true;
                inCredits = false;
            }
        }
        else if (inTop) {
            if (IsKeyPressed(KEY_RIGHT)) {
                viewingScores = true;
            }
            if (IsKeyPressed(KEY_LEFT)) {
                viewingScores = false;
            }
            if (IsKeyPressed(KEY_DOWN) && viewingScores) {
                topScoreChoice++;
                if (topScoreChoice > entries.size() - 1) {
                    topScoreChoice = 0;
                }
            }
            if (IsKeyPressed(KEY_UP) && viewingScores) {
                topScoreChoice--;
                if (topScoreChoice < 0) {
                    topScoreChoice = entries.size() - 1;
                }
            }
            
            if (IsKeyPressed(KEY_ENTER) && !viewingScores) {
                startScreen = true;
                inTop = false;
            }
            else if (IsKeyPressed(KEY_ENTER) && viewingScores) {
                viewingTop = true;
                inTop = false;
            }

        }
        else if (viewingTop) {
            if (IsKeyPressed(KEY_ENTER)) {
                inTop = true;
                viewingTop = false;
            }
        }
        else if (isDead) {
            if (IsKeyPressed(KEY_BACKSPACE) && nameLength > 0)
            {
                nameLength--;
                playerName[nameLength] = '\0';
            }
            if (IsKeyPressed(KEY_ENTER)) {
                SaveGame::SaveNameToFile(playerName, playerScore, enemiesKilled, ballomKills, onilKills, dahlKills, minvoKills, doriaKills, ovapeKills, passKills, pontanKills, bombsPlanted,softBlocksDestroyed,powerUpsPicked,fireUpCounter,bombUpCounter,speedUpCounter, flamePassCounter, bombPassCounter, InvincibleCounter, remoteControlCounter, wallPassCounter);
                entries = SaveGame::GetEntriesFromFile();

                std::sort(entries.begin(), entries.end(), [](const ScoreEntry& a, const ScoreEntry& b) {
                    return a.score > b.score;  // Ordenar por puntuación de mayor a menor
                    });

                std::vector<ScoreEntry> temp;
                if (entries.size() > MAX_TOPSCORES - 1) {
                    for (int i = 0; i < MAX_TOPSCORES - 1; i++) {
                        temp.push_back(entries[i]);
                    }
                    SaveGame::SaveEntriesToFile(temp);
                }
                else {
                    SaveGame::SaveEntriesToFile(entries);
                }
                
                startScreen = true;
                isDead = false;
            }

            for (int i = 0; i < 26; i++)  // 'a' a 'z'
            {
                if (IsKeyPressed(KEY_A + i))
                {
                    if (nameLength < sizeof(playerName) - 1)  
                    {
                        playerName[nameLength] = 'a' + i;
                        nameLength++;
                    }
                }
            }
        }

        Draw();
    }

    CloseWindow();
}

void Game::Update() {
    int prevX = player.GetX();
    int prevY = player.GetY();
    onBomb = CheckCollisions(player.GetBounds()) == 3;
    playerWalking = false;

    if (!player.IsActive()) {
        float duration = 3.0f; // Duración en segundos
        float timer = 0; // Temporizador para contar el tiempo transcurrido

        SeekMusicStream(resourceManager.GetMusic(4), 0.0f);
        PlayMusicStream(resourceManager.GetMusic(4));
        while (timer < duration) {
            UpdateMusicStream(resourceManager.GetMusic(4));
            Draw();
            timer += GetFrameTime();
        }
        if (player.GetLife() <= 0) {
            GameOver();
        }
        else {
            ResetStage();
        }
    }
    if (!player.IsDead()) {
        if (IsKeyDown(KEY_RIGHT)) {
            Vector2 v = { 1,0 };
            player.Move(PLAYER_SPEED, 0, v);
            playerWalking = true;
            if (!IsSoundPlaying(resourceManager.GetSound(0))) {
                PlaySound(resourceManager.GetSound(0));
            }
            if (CheckPlayerCollisions(player.GetBounds()) != 0) {
                player.SetX(prevX);
            }
            

        }
        if (IsKeyDown(KEY_LEFT)) {
            player.Move(-PLAYER_SPEED, 0, { -1, 0 });
            playerWalking = true;
            if (!IsSoundPlaying(resourceManager.GetSound(0))) {
                PlaySound(resourceManager.GetSound(0));
            }
            if (CheckPlayerCollisions(player.GetBounds()) != 0) {
                player.SetX(prevX);
            }
            

        }
        if (IsKeyDown(KEY_DOWN)) {
            player.Move(0, PLAYER_SPEED, { 0,-1 });
            playerWalking = true;
            if (!IsSoundPlaying(resourceManager.GetSound(1))) {
                PlaySound(resourceManager.GetSound(1));
            }
            if (CheckPlayerCollisions(player.GetBounds()) != 0) {
                player.SetY(prevY);
            }
            

        }
        if (IsKeyDown(KEY_UP)) {
            player.Move(0, -PLAYER_SPEED, { 0,1 });
            playerWalking = true;
            if (!IsSoundPlaying(resourceManager.GetSound(1))) {
                PlaySound(resourceManager.GetSound(1));
            }
            if (CheckPlayerCollisions(player.GetBounds()) != 0) {
                player.SetY(prevY);
            }
            

        }
    }
    Vector2 playerPos = {player.GetX(), player.GetY()};
    if (CheckEnemyCollision(playerPos.x, playerPos.y)) {
        if (!player.IsDead()) {
            PlaySound(resourceManager.GetSound(4));
            if (invincibleTimer <= 0) {
                player.Die();
            }
        }
    }

    if (CheckPlayerBlastDamage(playerPos)) {
        if(!player.IsDead()){
            PlaySound(resourceManager.GetSound(4));
            if (invincibleTimer <= 0) {
                player.Die();
            }
        }
    }
    if (playerWalking) {
        player.SetIdle(false);
    }
    else {
        player.SetIdle(true);
    }
    if (enemyCounter == 0) {
        CheckExitCollision();
    }

    CheckPowerUPCollision(player.GetBounds());
    player.Update();

    if (IsKeyPressed(KEY_ENTER)) { 
        AddBomb(player.GetBounds().x, player.GetBounds().y); 
    }

    if (isCoop) {
        int prevX2 = player2.GetX();
        int prevY2 = player2.GetY();
        playerWalking2 = false;

        if (!player2.IsActive()) {
            float duration = 3.0f; // Duración en segundos
            float timer = 0; // Temporizador para contar el tiempo transcurrido

            SeekMusicStream(resourceManager.GetMusic(4), 0.0f);
            PlayMusicStream(resourceManager.GetMusic(4));
            while (timer < duration) {
                UpdateMusicStream(resourceManager.GetMusic(4));
                Draw();
                timer += GetFrameTime();
            }
            if (player2.GetLife() <= 0) {
                GameOver();
            }
            else {
                ResetStage();
            }
        }
        if (!player2.IsDead()) {
            if (IsKeyDown(KEY_D)) {
                Vector2 v = { 1,0 };
                player2.Move(PLAYER_SPEED, 0, v);
                playerWalking2 = true;
                if (!IsSoundPlaying(resourceManager.GetSound(0))) {
                    PlaySound(resourceManager.GetSound(0));
                }
                if (CheckPlayerCollisions(player2.GetBounds()) != 0) {
                    player2.SetX(prevX2);
                }
                

            }
            if (IsKeyDown(KEY_A)) {
                player2.Move(-PLAYER_SPEED, 0, { -1, 0 });
                playerWalking2 = true;
                if (!IsSoundPlaying(resourceManager.GetSound(0))) {
                    PlaySound(resourceManager.GetSound(0));
                }
                if (CheckPlayerCollisions(player2.GetBounds()) != 0) {
                    player2.SetX(prevX2);
                }
                

            }
            if (IsKeyDown(KEY_S)) {
                player2.Move(0, PLAYER_SPEED, { 0,-1 });
                playerWalking2 = true;
                if (!IsSoundPlaying(resourceManager.GetSound(1))) {
                    PlaySound(resourceManager.GetSound(1));
                }
                if (CheckPlayerCollisions(player2.GetBounds()) != 0) {
                    player2.SetY(prevY2);
                }
                

            }
            if (IsKeyDown(KEY_W)) {
                player2.Move(0, -PLAYER_SPEED, { 0,1 });
                playerWalking2 = true;
                if (!IsSoundPlaying(resourceManager.GetSound(1))) {
                    PlaySound(resourceManager.GetSound(1));
                }
                if (CheckPlayerCollisions(player2.GetBounds()) != 0) {
                    player2.SetY(prevY2);
                }
                

            }
        }
        Vector2 playerPos2 = { player2.GetX(), player2.GetY() };
        if (CheckEnemyCollision(playerPos2.x, playerPos2.y)) {
            if (!player2.IsDead()) {
                PlaySound(resourceManager.GetSound(4));
                if (INVINCIBLE == false) {
                    player.Die();
                }
            }
        }

        if (CheckPlayerBlastDamage(playerPos2)) {
            if (!player2.IsDead()) {
                PlaySound(resourceManager.GetSound(4));
                if (INVINCIBLE == false) {
                    player.Die();
                }
            }
        }
        if (playerWalking2) {
            player2.SetIdle(false);
        }
        else {
            player2.SetIdle(true);
        }
        if (enemyCounter == 0) {
            CheckExitCollision();
        }

        CheckPowerUPCollision(player2.GetBounds());

        player2.Update();

        if (IsKeyPressed(KEY_SPACE)) {
            PlaySound(resourceManager.GetSound(2));
            AddBomb(player2.GetBounds().x, player2.GetBounds().y);
        }
    }
    if (IsKeyPressed(KEY_R)) {
        explodeB = true;
    }
    //////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Debug Mode
    //////////////////////////////////////////////////////////////////////////////////////////////////////////
    if (IsKeyPressed(KEY_E)) {
        for (auto it = enemies.begin(); it != enemies.end();) {
            (*it)->Die();
            ++it;             
        }
        for (auto it = softBlocks.begin(); it != softBlocks.end();) {
            it->Destroy();
            softBlocksDestroyed++;
            ++it; 
        }
    }
    if (IsKeyPressed(KEY_Y)) {
        GameOver();
    }
    if (IsKeyDown(KEY_P)) {
        playerScore += 10000;
    }
    if (IsKeyDown(KEY_B)) {
        PrepareBossAttack1();
    }
    //Mostrar Hitboxes
    if (IsKeyPressed(KEY_F1)) {
        if (debbugerMode1) {
            debbugerMode1 = false;
        }else{
            debbugerMode1 = true;
        }
    }
    //Mostrar Stats de Debug
    if (IsKeyPressed(KEY_F2)) {
        if (debbugerMode2) {
            debbugerMode2 = false;
        }
        else {
            debbugerMode2 = true;
        }
    }
    //Spawnear Enemigos en el Raton
    if (IsKeyPressed(KEY_F3)) {
        Vector2 pos = GetScreenToWorld2D(GetMousePosition(), camera1);
        pos.y += 50;
        pos.x = floorf(pos.x / CELL_SIZE) * CELL_SIZE;
        pos.y = floorf(pos.y / CELL_SIZE) * CELL_SIZE + 16;
        
        AddEnemy(pos, enemySelected);
    }
    //Rotar Enemigos a Spawnear
    if (IsKeyPressed(KEY_F4)) {
        enemySelected++;
        if (enemySelected >= 8) {
            enemySelected = 0;
        }
    }
    //Spawnear PowerUp en el Raton
    if (IsKeyPressed(KEY_F5)) {
        Vector2 pos = GetScreenToWorld2D(GetMousePosition(), camera1);
        pos.y += 50;
        pos.x = floorf(pos.x / CELL_SIZE) * CELL_SIZE;
        pos.y = floorf(pos.y / CELL_SIZE) * CELL_SIZE + 16;

        AddPowerUp(pos, powerUpSelected);
    }
    //Rotar PowerUp a Spawnear
    if (IsKeyPressed(KEY_F6)) {
        powerUpSelected++;
        if (powerUpSelected >= 8) {
            powerUpSelected = 0;
        }
    }
    if (IsKeyPressed(KEY_F7)) {
        Vector2 pos = GetScreenToWorld2D(GetMousePosition(), camera1);
        pos.y += 50;
        pos.x = floorf(pos.x / CELL_SIZE) * CELL_SIZE;
        pos.y = floorf(pos.y / CELL_SIZE) * CELL_SIZE + 16;

        AddBlasts1(pos);
    }
    if (IsKeyPressed(KEY_F8)) {
        NextLevel();
    }
    if (IsKeyPressed(KEY_ONE)) {
        if (!resolutionChanged) {
            SetWindowSize(1280, 720);
            resolutionChanged = true;
        }
        else {
            SetWindowSize(1920, 1080);
            resolutionChanged = false;
        }
    }
    //printf("%d", PUWP);
    //////////////////////////////////////////////////////////////////////////////////////////////////////////
    
    for (auto it = scores.begin(); it != scores.end(); ) {
        if ((*it)->time > 0) {
            (*it)->time -= GetFrameTime();
            ++it;
        }
        else {
            it = scores.erase(it); // erase devuelve el siguiente iterador válido
        }
    }

    if (remainingTime <= 0 && noTimeLeft == false) {
        NoTime();
        noTimeLeft = true;
    }

    for (auto it = blasts.begin(); it != blasts.end(); ) {
        it->Update(GetFrameTime()); 
        if (!it->active) {
            it = blasts.erase(it); // Eliminar blast si no está activo
        }
        else {
            ++it; 
        }
    }
    for (auto it = coils.begin(); it != coils.end(); ) {
        if (coils.size() > 0) {
            if ((*it)->IsDead() == false) {
                if (CheckBlastDamage({ ((*it)->GetPosition().x / CELL_SIZE) * CELL_SIZE, (((*it)->GetPosition().y + 200) / CELL_SIZE) * CELL_SIZE})) {
                    (*it)->Die(); // Llama a Die()
                    currentCoils--;
                }
            }
        }
        (*it)->Update(GetFrameTime());
         ++it;        
    }
    for (auto& powerup : powerups) {
        powerup->Update();
        if (CheckBlastDamage({powerup->GetBound().x, powerup->GetBound().y})) {
            if (extraEnemies == false) {
                AddEnemy({ powerup->GetBound().x, powerup->GetBound().y }, 1);
                enemies.back()->SetCanBeKilled(60);
                enemies.back()->SetSpeed(3.0f);
                AddEnemy({ powerup->GetBound().x, powerup->GetBound().y }, 1);
                enemies.back()->SetCanBeKilled(60);
                enemies.back()->SetSpeed(3.0f);
                AddEnemy({ powerup->GetBound().x, powerup->GetBound().y }, 1);
                enemies.back()->SetCanBeKilled(60);
                enemies.back()->SetSpeed(3.0f);
                AddEnemy({ powerup->GetBound().x, powerup->GetBound().y }, 1);
                enemies.back()->SetCanBeKilled(60);
                enemies.back()->SetSpeed(3.0f);
                extraEnemies = true;
            }
        }
    }
    if (currentStage != 5) {
        if (CheckBlastDamage({ exits.back().GetBound().x, exits.back().GetBound().y })) {
            if (extraEnemies == false) {
                AddEnemy({ exits.back().GetBound().x, exits.back().GetBound().y }, 1);
                enemies.back()->SetCanBeKilled(60);
                enemies.back()->SetSpeed(3.0f);
                AddEnemy({ exits.back().GetBound().x, exits.back().GetBound().y }, 1);
                enemies.back()->SetCanBeKilled(60);
                enemies.back()->SetSpeed(3.0f);
                AddEnemy({ exits.back().GetBound().x, exits.back().GetBound().y }, 1);
                enemies.back()->SetCanBeKilled(60);
                enemies.back()->SetSpeed(3.0f);
                AddEnemy({ exits.back().GetBound().x, exits.back().GetBound().y }, 1);
                enemies.back()->SetCanBeKilled(60);
                enemies.back()->SetSpeed(3.0f);
                extraEnemies = true;
            }
        }
    }

    for (auto it = bombs.begin(); it != bombs.end(); ) {
        float temp = (*it)->Update(GetFrameTime());
        
        if (temp <= 0) {
            if (!PURC) {
                PlaySound(resourceManager.GetSound(6));
                AddBlasts(**it);
                delete* it;
                it = bombs.erase(it);
            }
            else if(explodeB){
                PlaySound(resourceManager.GetSound(6));
                AddBlasts(**it);
                delete* it;
                it = bombs.erase(it);
            }
            else {
                ++it;
            }
             
        }
        else {
            ++it; 
        }
    }
    
    for (auto it = enemies.begin(); it != enemies.end();) {
        if (blasts.size() > 0) {
            if ((*it)->IsDead() == false) {
                if (CheckBlastDamage((*it)->GetPosition())) {
                    (*it)->Die(); // Llama a Die()
                }
            }
        }
        if (!(*it)->IsActive()) {
            switch ((*it)->id)
            {
            case 0:
                scores.push_back(std::make_unique<ScoreEnemies>(ScoreEnemies{ resourceManager.GetTexture(20), (*it)->GetPosition(), 0 , 20}));
                ballomKills++;
                enemiesKilled++;
                break;
            case 1:
                scores.push_back(std::make_unique<ScoreEnemies>(ScoreEnemies{ resourceManager.GetTexture(20), (*it)->GetPosition(), 1 , 20 }));
                onilKills++;
                enemiesKilled++;
                break;
            case 2:
                scores.push_back(std::make_unique<ScoreEnemies>(ScoreEnemies{ resourceManager.GetTexture(20), (*it)->GetPosition(), 2 , 20 }));
                dahlKills++;
                enemiesKilled++;
                break;
            case 3:
                scores.push_back(std::make_unique<ScoreEnemies>(ScoreEnemies{ resourceManager.GetTexture(20), (*it)->GetPosition(), 3 , 20 }));
                minvoKills++;
                enemiesKilled++;
                break;
            case 4:
                scores.push_back(std::make_unique<ScoreEnemies>(ScoreEnemies{ resourceManager.GetTexture(20), (*it)->GetPosition(), 4 , 20 }));
                doriaKills++;
                enemiesKilled++;
                break;
            case 5:
                scores.push_back(std::make_unique<ScoreEnemies>(ScoreEnemies{ resourceManager.GetTexture(20), (*it)->GetPosition(), 5 , 20 }));
                ovapeKills++;
                enemiesKilled++;
                break;
            case 6:
                scores.push_back(std::make_unique<ScoreEnemies>(ScoreEnemies{ resourceManager.GetTexture(20), (*it)->GetPosition(), 6 , 20 }));
                passKills++;
                enemiesKilled++;
                break;
            case 7:
                scores.push_back(std::make_unique<ScoreEnemies>(ScoreEnemies{ resourceManager.GetTexture(20), (*it)->GetPosition(), 7 , 20 }));
                pontanKills++;
                enemiesKilled++;
                break;
            default:
                break;
            }
            playerScore += (*it)->GetScore();
            it = enemies.erase(it); // Elimina el enemigo
            enemyCounter--;
        }
        else {
            
            (*it)->Update(GetFrameTime(), walls, softBlocks); // Actualiza si todavía está activo
            ++it; 
        }
    }
    for (auto it = softBlocks.begin(); it != softBlocks.end();) {
        if (it->IsDestroyed()) {
            it->Update();
            if (!it->IsActive()) {
                softBlocksDestroyed++;
                it = softBlocks.erase(it); 
                continue;
            }
        }
        ++it; 
    }
    if (currentStage == 5) {
        currentBoss->Update(GetFrameTime(), walls, softBlocks);

        bossAttackCooldown -= GetFrameTime();
        if (bossAttackCooldown <= 0) {

            int attackType = 0;
            if (attack1state == false) {
                if (currentPhase == 2) {
                    if (propagationState == false) {
                        attackType = rand() % 3;  // Número aleatorio entre 0 y 2
                    }
                    else {
                        attackType = 0;
                    }
                }
                else {
                    attackType = 0;
                }
                switch (attackType) {
                case 0:
                    PrepareBossAttack1();
                    break;
                case 1:
                    PrepareBossAttack2();
                    break;
                case 2:
                    PrepareBossAttack3();
                    break;
                }

                currentBoss->isAttacking = true;
                bossAttackCooldown = 3.0f;
            }
        }
        if (attack1state == true) {
            BossCoroutine(GetFrameTime());

        }

        if (propagationState) {
            propagationTime += GetFrameTime();;

            // Si ha pasado el tiempo necesario para expandir
            if (propagationTime >= expansionSpeed) {
                propagationTime = 0.0f;  // Resetear el tiempo

                // Aumentamos el radio
                currentRadius += 1;

                // Re-creamos el círculo y los blasts en la nueva zona
                CreateCircle(centerX, centerY, currentRadius);

                attack1state = true;

                // Si el radio ha alcanzado el máximo, terminamos la propagación
                if (currentRadius >= maxRadius) {
                    propagationState = false;
                }
            }
        }

        if (currentCoils <= 0) {
            currentPhase = 2;
            energyShield.reset();
        }
    }

    if (explodeB == true) {
        explodeB = false;
    }

    if (invincibleTimer > 0) {
        invincibleTimer -= GetFrameTime();
    }

    elapsedTime = GetTime() - startTime;
    remainingTime = targetTime - elapsedTime;
}

void Game::Draw() {
    BeginTextureMode(renderTarget);
   
    if (startScreen) {
        ClearBackground(BLACK);
        DrawTextureEx(initialScreen, {(float)SCREEN_WIDTH/2 - 650, 50},0,4, WHITE);
        DrawTextureEx(arrowTexture, { arrowPositions[menuChoice].x, arrowPositions[menuChoice].y}, 0, 4, WHITE);
    }
    else if(gameRunning){

        // Cámara 1
        
        Vector2 target1 = { player.GetBounds().x, SCREEN_HEIGHT/2 + 200};

        if (!isCoop) {
            camera1.zoom = 0.7f;

            target1.x = Clamp(target1.x, 700, 1350);

            camera1.target = target1;
            camera1.offset = { SCREEN_WIDTH / 4.0f, SCREEN_HEIGHT / 2.0f };

            BeginMode2D(camera1);
            ClearBackground(GRAY);
            DrawRectangle(0, 215, 3000, 1200, DARKGREEN);
            for (auto& wall : walls) wall.Draw();
            for (auto& exit : exits) exit.Draw();
            for (auto& softBlock : softBlocks) softBlock.Draw();
            for (const auto& powerup : powerups) powerup->Draw();
            for (auto& bomb : bombs) bomb->Draw();
            for (auto& blast : blasts) blast.Draw();
            for (const auto& enemy : enemies) enemy->Draw();
            if (player.IsActive()) player.Draw();
            if (debbugerMode1) {
                for (auto& wall : walls) wall.DrawHitbox();
                if (player.IsActive()) player.DrawHitbox();
                for (auto& exit : exits) exit.DrawHitbox();
                for (auto& softBlock : softBlocks) softBlock.DrawHitbox();
                for (const auto& powerup : powerups) powerup->DrawHitbox();
                for (auto& bomb : bombs) bomb->DrawHitbox();
                for (auto& blast : blasts) blast.DrawHitbox();
                for (const auto& enemy : enemies) enemy->DrawHitbox();
            }
            for (const auto& score : scores)
            {
                Vector2 v = { score->pos.x, score->pos.y };
                Rectangle source = { 0, 0, 16, 5 };
                Rectangle dest = { (score->pos.x), (score->pos.y), 16 * 6.3f , 5 * 6.3f };
                Vector2 v2 = { 1, 1 };
                //score->id = 4;
                switch (score->id)
                {
                case 1:
                    source.y = 5;
                    break;
                case 2:
                    source.y = 10;
                    break;
                case 3:
                    source.y = 15;
                    break;
                case 4:
                    source.y = 0;
                    source.x = 16;
                    break;
                case 5:
                    source.y = 5;
                    source.x = 16;
                    break;
                case 6:
                    source.y = 10;
                    source.x = 16;
                    break;
                case 7:
                    source.y = 15;
                    source.x = 16;
                    break;
                default:
                    break;
                }

                DrawTexturePro(score->textura, source, dest, v2, 0, WHITE);
            }
            
            if (currentStage == 5) {
                currentBoss->Draw();
                for each (Rectangle var in zoneMark)
                {
                    DrawRectangle(var.x,var.y,var.width,var.height,semiTransparentRed);
                }
                for (const auto& coil : coils) coil->Draw();
                if (currentPhase == 1) {
                    energyShield->Draw();
                }
            }
            EndMode2D();
        }else{
        // Cámara 2

            camera1.zoom = 0.7f;

            target1.x = Clamp(target1.x, 700, 2400);

            camera1.target = target1;
            camera1.offset = { SCREEN_WIDTH / 4.0f, SCREEN_HEIGHT / 2.0f };

            Vector2 target2 = { player2.GetBounds().x, SCREEN_HEIGHT / 2 + 200 };

            camera2.zoom = 0.7f;

            
            target2.x = Clamp(target2.x, 700, 2400);

            camera2.target = target2;
            camera2.offset = { SCREEN_WIDTH * 3.0f / 4.0f, SCREEN_HEIGHT / 2.0f };
            
        
        // Dibujo con cámara 1 (jugador 1)
        BeginScissorMode(0, 0, SCREEN_WIDTH / 2, SCREEN_HEIGHT);  // Delimita lado izquierdo
        BeginMode2D(camera1);
        ClearBackground(GRAY);
        DrawRectangle(0, 215, 4000, 1200, DARKGREEN);
        for (auto& wall : walls) wall.Draw();
        for (auto& exit : exits) exit.Draw();
        for (auto& softBlock : softBlocks) softBlock.Draw();
        for (const auto& powerup : powerups) powerup->Draw();
        for (auto& bomb : bombs) bomb->Draw();
        for (auto& blast : blasts) blast.Draw();
        for (const auto& enemy : enemies) enemy->Draw();
        for (const auto& coil : coils) coil->Draw();
        if (player.IsActive()) player.Draw();
        if (player2.IsActive()) player2.Draw();
        if (currentStage == 5) {
            currentBoss->Draw();
            for each (Rectangle var in zoneMark)
            {
                DrawRectangle(var.x, var.y, var.width, var.height, semiTransparentRed);
            }
            for (const auto& coil : coils) coil->Draw();
            if (currentPhase == 1) {
                energyShield->Draw();
            }
        }
        EndMode2D();
        EndScissorMode();

        // Dibujo con cámara 2 (jugador 2)
        
            BeginScissorMode(SCREEN_WIDTH / 2, 0, SCREEN_WIDTH / 2, SCREEN_HEIGHT);  // Delimita lado derecho
            BeginMode2D(camera2);
            ClearBackground(GRAY);
            DrawRectangle(0, 215, 4000, 1200, DARKGREEN);
            for (auto& wall : walls) wall.Draw();
            for (auto& exit : exits) exit.Draw();
            for (auto& softBlock : softBlocks) softBlock.Draw();
            for (const auto& powerup : powerups) powerup->Draw();
            for (auto& bomb : bombs) bomb->Draw();
            for (auto& blast : blasts) blast.Draw();
            for (const auto& enemy : enemies) enemy->Draw();
            if (player.IsActive()) player.Draw();
            if (player2.IsActive()) player2.Draw();
            if (currentStage == 5) {
                currentBoss->Draw();
                for each (Rectangle var in zoneMark)
                {
                    DrawRectangle(var.x, var.y, var.width, var.height, semiTransparentRed);
                }
                for (const auto& coil : coils) coil->Draw();
                if (currentPhase == 1) {
                    energyShield->Draw();
                }
            }
            EndMode2D();
            EndScissorMode();
        }
        if (isCoop) {
            DrawRectangle(SCREEN_WIDTH / 2 - 2, 0, 4, SCREEN_HEIGHT, BLACK);
        }

        if (debbugerMode2) {
            DrawRectangle(0,0,1920, 1080, { 128, 128, 128, 160 });
            DrawText(TextFormat("Posicion Jugador: %.2f, %.2f", player.GetBounds().x, player.GetBounds().y), 600, 300, 40, WHITE);
            DrawText(TextFormat("Velocidad Jugador: %d", PLAYER_SPEED), 600, 340, 40, WHITE);
            if (isCoop) {
                DrawText(TextFormat("Posicion Jugador 2: %.2f, %.2f", player2.GetBounds().x, player2.GetBounds().y), 600, 340, 40, WHITE);
            }
            DrawText(TextFormat("Enemigos Totales: %d", totalEnemiesThisStage), 600, 380, 40, WHITE);
            DrawText(TextFormat("Enemigos Restantes: %d", enemies.size()), 600, 420, 40, WHITE);
            DrawText(TextFormat("Bombas Disponibles: %d", MAX_BOMBS - bombs.size()), 600, 460, 40, WHITE);
            DrawText(TextFormat("Rango Bombas: %d", BOMB_RANGE), 600, 500, 40, WHITE);
            DrawText(TextFormat("PowerUps Recogidos: %d", powerUpsPicked), 600, 540, 40, WHITE);


        }
        // HUD común (fuera de las cámaras)
        DrawText(TextFormat("TIME %.0f", remainingTime), 30, 80, 40, WHITE);
        DrawText(TextFormat("SCORE %d", playerScore), SCREEN_WIDTH / 2 - 200, 80, 40, WHITE);
        DrawText(TextFormat("LEFT %d", player.GetLife()), SCREEN_WIDTH - 500, 80, 40, WHITE);
        DrawFPS(20, SCREEN_HEIGHT - 30);
    }
    else if (inCredits) {
        ClearBackground(BLACK);
        DrawText("Proyecto: Bomberman (NES)", 80, 60, 60, WHITE);
        DrawText("Version clasica del iconico Bomberman de NES, donde el jugador debe colocar \nbombas estrategicamente para eliminar enemigos y abrirse paso a traves de un \nlaberinto de bloques. El objetivo es superar cada nivel utilizando logica,\nreflejos y planificacion, en una mezcla de accion y puzles retro.", 100, 150, 30, WHITE);
        DrawText("Asignatura: Proyecto 1", 100, 310, 30, WHITE);
        DrawText("Grado en Diseno y Desarrollo de Videojuegos", 100, 350, 30, WHITE);
        DrawText("Universidad Politecnica de Catalunya", 100, 390, 30, WHITE);

        DrawText("Equipo de Desarrollo:", 100, 450, 35, WHITE);
        DrawText("- Marc Gonzalez", 120, 490, 30, WHITE);
        DrawText("- Lluis Cazorla", 120, 530, 30, WHITE);
        DrawText("- Santiago Alvarez", 120, 570, 30, WHITE);

        DrawText("Tutores:", 100, 630, 35, WHITE);
        DrawText("- Prof. Alejandro Paris", 120, 670, 30, WHITE);

        DrawText("Mas Informacion:", 100, 730, 35, WHITE);
        DrawText("GitHub: github.com/MarcGonzalezCarro/Projecte1", 120, 770, 30, WHITE);

        DrawText("Back", 120, 840, 40, WHITE);
        DrawTextureEx(arrowTexture, { 80, 840 }, 0, 4, WHITE);
    }
    else if (inTop) {
        ClearBackground(BLACK);
        DrawText("Top Scores", (float)SCREEN_WIDTH / 2 - 200, 100, 40, WHITE);
        int yPosition = 300; // Posición inicial en Y para mostrar las entradas
        for (const auto& entry : entries) {
            std::string displayText = entry.name + " - " + std::to_string(entry.score);
            DrawText(displayText.c_str(), (float)SCREEN_WIDTH / 2 - 200, yPosition, 30, WHITE);
            yPosition += 40; // Mover hacia abajo para la siguiente entrada
        }

        DrawText("Back", 140, 700, 40, WHITE);
        if (!viewingScores) {
            DrawTextureEx(arrowTexture, { 100, 700 }, 0, 4, WHITE);
        }
        else {
            DrawTextureEx(arrowTexture, { 700,(float) 300 + 40 * topScoreChoice}, 0, 4, WHITE);
        }
    }
    else if (viewingTop) {
        ClearBackground(BLACK);

        // Asegurarse de que topScoreChoice esté dentro de los límites
        if (topScoreChoice >= 0 && topScoreChoice < entries.size()) {
            const ScoreEntry& selectedEntry = entries[topScoreChoice];

            // Mostrar el nombre y el puntaje del jugador seleccionado
            DrawText(("Name: " + selectedEntry.name).c_str(), 850, 100, 30, WHITE);
            DrawText(("Score: " + std::to_string(selectedEntry.score)).c_str(), 850, 150, 30, WHITE);

            // Mostrar las estadísticas del jugador
            DrawText(("Enemies Killed: " + std::to_string(selectedEntry.enemiesKilled)).c_str(), 600, 200, 30, WHITE);
            DrawText(("Ballom Kills: " + std::to_string(selectedEntry.ballomKills)).c_str(), 600, 250, 30, WHITE);
            DrawText(("Onil Kills: " + std::to_string(selectedEntry.onilKills)).c_str(), 600, 300, 30, WHITE);
            DrawText(("Dahl Kills: " + std::to_string(selectedEntry.dahlKills)).c_str(), 600, 350, 30, WHITE);
            DrawText(("Minvo Kills: " + std::to_string(selectedEntry.minvoKills)).c_str(), 600, 400, 30, WHITE);
            DrawText(("Doria Kills: " + std::to_string(selectedEntry.doriaKills)).c_str(), 600, 450, 30, WHITE);
            DrawText(("Ovape Kills: " + std::to_string(selectedEntry.ovapeKills)).c_str(), 600, 500, 30, WHITE);
            DrawText(("Pass Kills: " + std::to_string(selectedEntry.passKills)).c_str(), 600, 550, 30, WHITE);
            DrawText(("Pontan Kills: " + std::to_string(selectedEntry.pontanKills)).c_str(), 600, 600, 30, WHITE);
            DrawText(("Bombs Planted: " + std::to_string(selectedEntry.bombsPlanted)).c_str(), 850, 650, 30, WHITE);
            DrawText(("Soft Blocks Destroyed: " + std::to_string(selectedEntry.softBlocksDestroyed)).c_str(), 850, 700, 30, WHITE);
            DrawText(("Power-Ups Picked: " + std::to_string(selectedEntry.powerUpsPicked)).c_str(), 1100, 200, 30, WHITE);
            DrawText(("Fire-Up Counter: " + std::to_string(selectedEntry.fireUpCounter)).c_str(), 1150, 250, 30, WHITE);
            DrawText(("Bomb-Up Counter: " + std::to_string(selectedEntry.bombUpCounter)).c_str(), 1150, 300, 30, WHITE);
            DrawText(("Speed-Up Counter: " + std::to_string(selectedEntry.speedUpCounter)).c_str(), 1150, 350, 30, WHITE);
            DrawText(("Flame-Pass Counter: " + std::to_string(selectedEntry.flamePassCounter)).c_str(), 1150, 400, 30, WHITE);
            DrawText(("Bomb-Pass Picked: " + std::to_string(selectedEntry.bombPassCounter)).c_str(), 1150, 450, 30, WHITE);
            DrawText(("Invincible Counter: " + std::to_string(selectedEntry.InvincibleCounter)).c_str(), 1150, 500, 30, WHITE);
            DrawText(("Remote-Control Counter: " + std::to_string(selectedEntry.remoteControlCounter)).c_str(), 1150, 550, 30, WHITE);
            DrawText(("Wall-Pass Counter: " + std::to_string(selectedEntry.wallPassCounter)).c_str(), 1150, 600, 30, WHITE);

            // Volver al menú principal o salir
            DrawText("Back", 140, 700, 40, WHITE);
            if (viewingScores) {
                DrawTextureEx(arrowTexture, { 100, 700 }, 0, 4, WHITE);
            }
        }
    }
    /*else if (isDead) {
        ClearBackground(BLACK);
        DrawText("GAME OVER", (float)SCREEN_WIDTH / 2 - 200, 100, 40, WHITE);

        // Dibujar la línea de guiones bajos y reemplazarlos por las letras
        for (int i = 0; i < 5; i++) // Mostrar un máximo de 5 caracteres
        {
            if (i < nameLength)
                DrawText(TextFormat("%c", playerName[i]), (float)SCREEN_WIDTH / 2 - 200 + i * 30, 250, 30, WHITE);
            else
                DrawText("_", (float)SCREEN_WIDTH / 2 - 200 + i * 30, 250, 30, WHITE); // Mostrar guiones bajos
        }
    }*/
    else if (isDead)
    {
        ClearBackground(BLACK);
        DrawTextureEx(endScreen, {0, 0}, 0, 1, WHITE);
        DrawText("INSERT YOUR NAME:", (float)SCREEN_WIDTH / 2 - 450, 850, 40, WHITE);
        for (int i = 0; i < 5; i++) // Mostrar un máximo de 5 caracteres
        {
            if (i < nameLength)
                DrawText(TextFormat("%c", playerName[i]), (float)SCREEN_WIDTH / 2 + i * 30, 850, 30, WHITE);
            else
                DrawText("_", (float)SCREEN_WIDTH / 2 + i * 30, 860, 30, WHITE); // Mostrar guiones bajos
        }


    }
    EndTextureMode();

    BeginDrawing();
    ClearBackground(BLACK);

    float screenW = GetScreenWidth();
    float screenH = GetScreenHeight();
    float scale = fminf(screenW / (float)INTERNAL_WIDTH, screenH / (float)INTERNAL_HEIGHT);

    Rectangle src = { 0, 0, (float)renderTarget.texture.width, -(float)renderTarget.texture.height }; // Flip vertical
    Rectangle dest = {
        (screenW - INTERNAL_WIDTH * scale) / 2.0f,
        (screenH - INTERNAL_HEIGHT * scale) / 2.0f,
        INTERNAL_WIDTH * scale,
        INTERNAL_HEIGHT * scale
    };

    DrawTexturePro(renderTarget.texture, src, dest, { 0, 0 }, 0.0f, WHITE);

    EndDrawing();
}

void Game::AddWalls() {
    walls.clear();
    softBlocks.clear();

    if (currentStage != 5) {
        for (int i = 0; i < 31; i++) {
            for (int j = 0; j < 13; j++) {
                if (i != 0 && j != 0 && i != 31 - 1 && j != 13 - 1) {
                    if (i % 2 == 0 && j % 2 == 0) {
                        walls.emplace_back(i * CELL_SIZE, j * CELL_SIZE + SCREEN_HEIGHT / 5);
                        walls.back().SetTexture(resourceManager.GetTexture(12));
                    }
                    else if ((i <= 3 && j <= 3)) {
                        //Lugar SEguro
                    }
                    else {
                        if (GetRandomValue(1, 4) == 1) {
                            
                            softBlocks.emplace_back(i * CELL_SIZE, j * CELL_SIZE + SCREEN_HEIGHT / 5);
                            softBlocks.back().SetTexture(resourceManager.GetTexture(12));
                            
                        }
                    }
                }

                else {
                    walls.emplace_back(i * CELL_SIZE, j * CELL_SIZE + SCREEN_HEIGHT / 5);
                    walls.back().SetTexture(resourceManager.GetTexture(12));
                }
            }

        }

        int temp = GetRandomValue(0, softBlocks.size() - 1);
        exits.emplace_back(softBlocks.at(temp).GetBound().x, softBlocks.at(temp).GetBound().y);
        exits.back().SetTexture(resourceManager.GetTexture(14));
        printf("Salida en: %f, %f\n", exits.back().GetBound().x, exits.back().GetBound().y);

        temp = GetRandomValue(0, softBlocks.size() - 1);
        while (softBlocks.at(temp).GetBound().x == exits.back().GetBound().x && softBlocks.at(temp).GetBound().y == exits.back().GetBound().y) {
            temp = GetRandomValue(0, softBlocks.size() - 1);
        }
        softBlocks.at(temp).hasPowerUp = true;
        printf("PowerUp en %f, %f", softBlocks.at(temp).GetBound().x, softBlocks.at(temp).GetBound().y);

        for (int i = 1; i < 31 - 1; i++) {
            for (int j = 1; j < 13 - 1; j++) {
                // Evitar esquinas seguras, como hacías antes
                if (i <= 3 && j <= 3) continue;
                // Evitar muros sólidos
                if (i % 2 == 0 && j % 2 == 0) continue;

                Vector2 pos = { (float)(i * CELL_SIZE), (float)(j * CELL_SIZE + SCREEN_HEIGHT / 5) };
                emptyPositions.push_back(pos);
            }
        }

        auto isOccupied = [&](const Vector2& pos) {
            for (const auto& wall : walls)
                if (CheckCollisionRecs(wall.GetBound(), { pos.x, pos.y, (float)CELL_SIZE,  (float)CELL_SIZE }))
                    return true;

            for (const auto& block : softBlocks)
                if (CheckCollisionRecs(block.GetBound(), { pos.x, pos.y,  (float)CELL_SIZE,  (float)CELL_SIZE }))
                    return true;

            for (const auto& exit : exits)
                if (CheckCollisionRecs(exit.GetBound(), { pos.x, pos.y,  (float)CELL_SIZE,  (float)CELL_SIZE }))
                    return true;

            return false;
        };

        // Eliminar posiciones ocupadas
        emptyPositions.erase(
            std::remove_if(emptyPositions.begin(), emptyPositions.end(), isOccupied),
            emptyPositions.end()
        );

        for (int enemyType = 0; enemyType < 8; ++enemyType) {
            int count = customizer.GetEnemyCounter(currentStage, enemyType);

            for (int i = 0; i < count && !emptyPositions.empty(); ++i) {
                int index = GetRandomValue(0, emptyPositions.size() - 1);
                Vector2 pos = emptyPositions[index];

                AddEnemy(pos, enemyType); // <- tu método personalizado
                totalEnemiesThisStage++;
                emptyPositions.erase(emptyPositions.begin() + index); // eliminar posición usada
            }
        }
    }
    else {
        for (int i = 0; i < 31; i++) {
            for (int j = 0; j < 13; j++) {
                if (i == 6 && j == 4) continue;
                if (i == 24 && j == 4) continue;
                if (i == 14 && j == 10) continue;
                if (i == 16 && j == 10) continue;
                if (i != 0 && j != 0 && i != 31 - 1 && j != 13 - 1) {
                    if (i % 2 == 0 && j % 2 == 0 && (i <= 11 || i >= 19 || j >= 5)) {
                        walls.emplace_back(i * CELL_SIZE, j * CELL_SIZE + SCREEN_HEIGHT / 5);
                        walls.back().SetTexture(resourceManager.GetTexture(12));
                    }
                }else {
                    walls.emplace_back(i * CELL_SIZE, j * CELL_SIZE + SCREEN_HEIGHT / 5);
                    walls.back().SetTexture(resourceManager.GetTexture(12));
                }
            }

        }
        
    }
   
}


void Game::AddBomb(float x, float y) {
    bool temp = false;
    for (auto it = bombs.begin(); it != bombs.end(); ) {
        if ((*it)->GetBounds().x == (float)(((int)(x) / CELL_SIZE) * CELL_SIZE) && (*it)->GetBounds().y == (float)(((int)(y) / CELL_SIZE) * CELL_SIZE) + 15) {
            temp = true;
            printf("Aqui ya hay bomba");
            break;
        }
        it++;
    }
    
    if (bombs.empty()) {
        
            if (bombs.size() < MAX_BOMBS) {

                bombs.push_back(new Bomb((((int)(x) / CELL_SIZE) * CELL_SIZE), (((int)(y) / CELL_SIZE) * CELL_SIZE) + 15));
                bombs.back()->SetTexture(resourceManager.GetTexture(3));
                PlaySound(resourceManager.GetSound(2));
                bombsPlanted++;
            }
        
    }
    else {
        if (!temp) {
            //printf("Se ha comparado %f con %f y %f con %f\n", (*it)->GetBounds().x, (float)(((int)(x) / CELL_SIZE) * CELL_SIZE), (*it)->GetBounds().y, (float)(((int)(y) / CELL_SIZE) * CELL_SIZE) + 15);
            if (bombs.size() < MAX_BOMBS) {

                bombs.push_back(new Bomb((((int)(x) / CELL_SIZE) * CELL_SIZE), (((int)(y) / CELL_SIZE) * CELL_SIZE) + 15));
                bombs.back()->SetTexture(resourceManager.GetTexture(3));
                PlaySound(resourceManager.GetSound(2));
                bombsPlanted++;
            }
        }
    }
    
}

void Game::ExplodeBombs() {
    for (auto& bomb : bombs) {
        //bomb.Explode();
    }
    bombs.clear();
}

void Game::AddBlasts(Bomb bomb) {
    Vector2 bombPos = { bomb.GetX(), bomb.GetY() };

    blasts.push_back(Blast(bombPos));
    blasts.back().SetTexture(resourceManager.GetTexture(2));
    blasts.back().direction = 4;
    

    // Direcciones: {arriba, abajo, izquierda, derecha}
    Vector2 directions[4] = { {0, -CELL_SIZE}, {0, CELL_SIZE}, {-CELL_SIZE, 0}, {CELL_SIZE, 0} };
    int temp = 0;
    int temp2 = 0;
    for (auto& dir : directions) {
        temp2 = 0;

        for (int i = 1; i <= bomb.GetRange(); i++) {
            Vector2 newPos = { bombPos.x + dir.x * i, bombPos.y + dir.y * i };
            
            if (IsBlastBlocked(newPos)) {
                // Si choca con una pared, agregamos un último blast en esa posición y paramos
                if (temp2 > 0) {
                    blasts.back().direction = temp;
                    blasts.back().SetFinal(true);
                }
                break;
            }

            blasts.push_back(Blast(newPos));
            blasts.back().direction = temp;
            blasts.back().SetTexture(resourceManager.GetTexture(2));
            if (temp2 == bomb.GetRange() - 1) {
                blasts.back().SetFinal(true);
            }
            else {
                blasts.back().SetFinal(false);
            }
            
            temp2++;
        }
        temp++;
    }
}

void Game::AddBlasts1(Vector2 pos)
{
    blasts.push_back(Blast(pos));
    blasts.back().SetTexture(resourceManager.GetTexture(2));
    blasts.back().direction = 4;
    blasts.back().SetFromBoss(true);

}

void Game::AddPowerUp(Vector2 pos, int powerUpType)
{
    switch (powerUpType)
    {
    case 0:
        powerups.push_back(std::make_unique<SpeedUp>(pos.x, pos.y));
        
        break;
    case 1:
        powerups.push_back(std::make_unique<BombUp>(pos.x, pos.y));
        
        break;
    case 2:
        powerups.push_back(std::make_unique<FireUp>(pos.x, pos.y));
        
        break;
    case 3:
        powerups.push_back(std::make_unique<WallPass>(pos.x, pos.y));
        
        break;
    case 4:
        powerups.push_back(std::make_unique<BombPass>(pos.x, pos.y));
        
        break;
    case 5:
        powerups.push_back(std::make_unique<FlamePass>(pos.x, pos.y));
        
        break;
    case 6:
        powerups.push_back(std::make_unique<Invincible>(pos.x, pos.y));
        
        break;
    case 7:
        powerups.push_back(std::make_unique<RemoteControl>(pos.x, pos.y));

        break;
    default:
        powerups.push_back(std::make_unique<SpeedUp>(pos.x, pos.y));

        break;
    }
    powerups.back()->SetTexture(resourceManager.GetTexture(13));
}

bool Game::IsBlastBlocked(Vector2 position) {
    for (const auto& wall : walls) {
        if (CheckCollisionRecs({ position.x, position.y + 1, (float) CELL_SIZE, (float)CELL_SIZE}, wall.GetBound())) {
            
            return true;  // Blockeado 
            
        }
    }
    for (auto it = softBlocks.begin(); it != softBlocks.end(); ++it) {
        if (CheckCollisionRecs({ position.x, position.y + 1, (float)CELL_SIZE, (float)CELL_SIZE }, it->GetBound())) {
            
            if (it->hasPowerUp == true) {
                AddPowerUp({ it->GetBound().x, it->GetBound().y }, customizerPU.GetPowerUpForStage(currentStage));
            }
            //printf("Añadido en: %f, %f\n", (*it).GetBound().x, (*it).GetBound().y);
            it->Destroy();
            return true;  // Bloqueado
        }
    }
    return false;  // No Blockeado :D
}

void Game::NoTime() {
    for (int i = 0; i < 9; i++)
    {
        int index = GetRandomValue(0, emptyPositions.size() - 1);
        Vector2 pos = emptyPositions[index];

        AddEnemy(pos, 7); 
        enemies.back()->SetSpeed(3.0f);
    }
}

void Game::AddEnemy(Vector2 pos, int enemyType) {
    switch (enemyType) {
    case 0:
        enemies.push_back(std::make_unique<Ballom>(pos)); // Crear y agregar Ballom
        enemies.back()->SetTexture(resourceManager.GetTexture(4));
        printf("Ballom");
        break;

    case 1:
        enemies.push_back(std::make_unique<Onil>(pos)); // Crear y agregar Onil
        enemies.back()->SetTexture(resourceManager.GetTexture(5));
        enemies.back()->SetSpeed(1.3f);
        break;

    case 2:
        enemies.push_back(std::make_unique<Dahl>(pos)); // Crear y agregar Dahl
        enemies.back()->SetTexture(resourceManager.GetTexture(6));
        enemies.back()->SetSpeed(1.3f);
        break;

    case 3:
        enemies.push_back(std::make_unique<Minvo>(pos)); // Crear y agregar Minvo
        enemies.back()->SetTexture(resourceManager.GetTexture(7));
        enemies.back()->SetSpeed(1.5f);
        break;

    case 4:
        enemies.push_back(std::make_unique<Doria>(pos)); // Crear y agregar Doria
        enemies.back()->SetTexture(resourceManager.GetTexture(8));
        enemies.back()->SetSpeed(0.6f);
        break;

    case 5:
        enemies.push_back(std::make_unique<Ovape>(pos)); // Crear y agregar Ovape
        enemies.back()->SetTexture(resourceManager.GetTexture(9));
        break;

    case 6:
        enemies.push_back(std::make_unique<Pass>(pos)); // Crear y agregar Pass
        enemies.back()->SetTexture(resourceManager.GetTexture(10));
        enemies.back()->SetSpeed(1.8f);
        break;

    case 7:
        enemies.push_back(std::make_unique<Pontan>(pos)); // Crear y agregar Pontan
        enemies.back()->SetTexture(resourceManager.GetTexture(11));
        enemies.back()->SetSpeed(2.0f);
        break;

    default:
        std::cerr << "Tipo de enemigo no válido: " << enemyType << std::endl;
        break;
    }

    enemyCounter++;
}


void Game::AddBoss(Vector2 pos)
{
    currentBoss = std::make_unique<MechaBlastX>(pos);
    currentBoss->SetTexture(resourceManager.GetTexture(17));
    AddCoil({ 580, 425});
    AddCoil({ 1480, 1025 });
    AddCoil({ 2380, 425 });
    AddEnergyShield({ 1400, 400 });
    
}

void Game::AddCoil(Vector2 pos)
{
    coils.push_back(std::make_unique<Coil>(pos));
    coils.back()->SetTexture(resourceManager.GetTexture(19));
    
}

void Game::AddEnergyShield(Vector2 pos)
{
    energyShield = std::make_unique<EnergyShield>(pos);
    energyShield->SetTexture(resourceManager.GetTexture(18));
}

int Game::CheckCollisions(Rectangle rec) {
    for (const auto& wall : walls) {
        if (CheckCollisionRecs(rec, wall.GetBound())) return 1;
    }
    for (const auto& softBlock : softBlocks) {
        if (CheckCollisionRecs(rec, softBlock.GetBound())) return 2;
    }
    
    return 0;
}

int Game::CheckPlayerCollisions(Rectangle rec) {
    for (const auto& wall : walls) {
        if (CheckCollisionRecs(rec, wall.GetBound())) return 1;
    }
    if (PUWP == false)
    {
        for (const auto& softBlock : softBlocks) {
            if (CheckCollisionRecs(rec, softBlock.GetBound())) return 2;
        }
    }
    
    return 0;
}

void Game::CheckExitCollision() {
    if (currentStage != 5) {
        Rectangle p = { player.GetBounds().x, player.GetBounds().y, player.GetBounds().width, player.GetBounds().height };
        Rectangle e = { exits.at(0).GetBound().x, exits.at(0).GetBound().y, CELL_SIZE - 20, CELL_SIZE - 20 };
        if (CheckCollisionRecs(p, e)) {
            if ((currentStage + 1) % 5 == 0) {
                BossLevel();
            }
            else {
                NextLevel();
            }
        }
    }

}

bool Game::CheckEnemyCollision(float x, float y) {
    if (currentStage != 5) {

        Rectangle p = { x, y, player.GetBounds().width, player.GetBounds().height };

        for (const auto& enemy : enemies) {
            Rectangle e = { enemy->GetPosition().x, enemy->GetPosition().y, CELL_SIZE - 20, CELL_SIZE - 20 };
            if (CheckCollisionRecs(p, e)) return true;
        }
        return false;
    }
    else {
        return false;
    }
}


void Game::CheckPowerUPCollision(Rectangle rec) {
    for (auto it = powerups.begin(); it != powerups.end();) {
        if (CheckCollisionRecs(rec, (*it)->GetBound())) {
            (*it)->Effect(); // Aplicar el efecto
            switch ((*it)->id)
            {
            case 0:
                bombUpCounter++;
                break;
            case 1:
                fireUpCounter++;
                break;
            case 2:
                speedUpCounter++;
                break;
            case 3:
                flamePassCounter++;
                break;
            case 4:
                bombPassCounter++;
                break;
            case 5:
                wallPassCounter++;
                break;
            case 6:
                remoteControlCounter++;
                break;
            case 7:
                InvincibleCounter++;
                invincibleTimer = 1500;
                break;
            default:
                break;
            }
            powerUpsPicked++;
            // Eliminar el PowerUp del vector después de aplicar el efecto
            it = powerups.erase(it); // Esto elimina el elemento y avanza al siguiente
        }
        else {
            ++it; // Solo avanza al siguiente si no se eliminó el actual
        }
    }
}

bool Game::CheckBlastDamage(Vector2 pos) {
    Rectangle rec = { pos.x, pos.y, CELL_SIZE, CELL_SIZE};
    for (const auto& blast : blasts) {
        Rectangle r = {blast.position.x, blast.position.y, CELL_SIZE - 20, CELL_SIZE - 20};
        if (CheckCollisionRecs(rec, r)) { 
            printf("Te ha dao en: %f,%f y player: %f, %f\n", blast.position.x, blast.position.y, rec.x,rec.y);
            return true; 
        }
    }
    return false;
}

bool Game::CheckPlayerBlastDamage(Vector2 pos) {
    if (!PUFP)
    {
        Rectangle rec = { pos.x, pos.y, CELL_SIZE - 10, CELL_SIZE - 10};
        for (const auto& blast : blasts) {
            Rectangle r = { blast.position.x, blast.position.y, CELL_SIZE - 20, CELL_SIZE - 20 };
            if (CheckCollisionRecs(rec, r)) {
                printf("Te ha dao en: %f,%f y player: %f, %f\n", blast.position.x, blast.position.y, rec.x, rec.y);
                return true;
            }
        }
    }
    return false;
}

void Game::ResetStage() {
    gameRunning = false;
    extraEnemies = false;
    noTimeLeft = false;
    totalEnemiesThisStage = 0;
    startTime = GetTime();
    targetTime = 200.0f;
    blasts.clear();
    bombs.clear();
    enemies.clear();
    exits.clear();
    powerups.clear();
    enemyCounter = 0;
    player.SetPosition(INITIAL_PLAYER_X, INITIAL_PLAYER_Y);
    player.DecreaseLife();
    player.SetIsDead(false);
    player.SetActive(true);
    if (isCoop) {
        player2.SetPosition(INITIAL_PLAYER_X, INITIAL_PLAYER_Y);
        player2.DecreaseLife();
        player2.SetIsDead(false);
        player2.SetActive(true);
    }
    AddWalls();
    //Empezar subrutina
    ShowStageScreen("Stage ");
    startTime = GetTime();
    targetTime = 200.0f;
    PUWP = false;
    INVINCIBLE = false;
}

void Game::ShowStageScreen(const char* stageText) {
    // Establecer el tiempo objetivo: 2 segundos
    const float duration = 3.0f; // Duración en segundos
    float timer = 0; // Temporizador para contar el tiempo transcurrido
    
    StopMusicStream(resourceManager.GetMusic(3));
    PlayMusicStream(resourceManager.GetMusic(3));

    while (timer < duration) {   // Mientras el tiempo transcurrido sea menor que 2 segundos
        BeginDrawing();
        ClearBackground(BLACK);

        UpdateMusicStream(resourceManager.GetMusic(3));

        int screenWidth = GetScreenWidth();
        int screenHeight = GetScreenHeight();
        int textWidth = MeasureText(stageText, 50); // Mide el ancho del texto
        int textX = 750;
        int textY = screenHeight / 2;

        if (timer > 0.2f && timer < 2.8f) {
            DrawText(TextFormat("%s %d", stageText, currentStage), textX, textY, 50, WHITE);
        }
        EndDrawing();

        timer += GetFrameTime(); // Incrementa el temporizador basado en el tiempo por frame
    }
    gameRunning = true;
}

void Game::NextLevel() {
    currentStage++;
    gameRunning = false;

    noTimeLeft = false;
    extraEnemies = false;
    blasts.clear();
    bombs.clear();
    enemies.clear();
    exits.clear();
    powerups.clear();
    enemyCounter = 0;
    player.SetPosition(INITIAL_PLAYER_X, INITIAL_PLAYER_Y);
    if (isCoop) {
        player2.SetPosition(INITIAL_PLAYER_X, INITIAL_PLAYER_Y);
    }
    AddWalls();
    //Empezar subrutina
    ShowStageScreen("Stage ");
    startTime = GetTime();
    targetTime = 200.0f;
}


void Game::BossLevel()
{
    currentStage++;
    gameRunning = false;



    blasts.clear();
    bombs.clear();
    enemies.clear();
    exits.clear();
    powerups.clear();
    enemyCounter = 0;
    player.SetPosition(INITIAL_PLAYER_X + (CELL_SIZE * 14), INITIAL_PLAYER_X + (CELL_SIZE * 12));
    if (isCoop) {
        player2.SetPosition(INITIAL_PLAYER_X, INITIAL_PLAYER_Y);
    }
    AddWalls();
    //Empezar subrutina
    ShowStageScreen("Stage ");
    startTime = GetTime();
    targetTime = 200.0f;
}

void Game::PrepareBossAttack1()
{
    for (int i = -1; i <= 1; ++i) {
        for (int j = -1; j <= 1; ++j) {
            Vector2 position = { player.GetBounds().x + i * CELL_SIZE, player.GetBounds().y + j * CELL_SIZE}; // Multiplicamos por 50 para espaciar las casillas
            position.x = (int)position.x / CELL_SIZE * CELL_SIZE;
            position.y = (int)position.y / CELL_SIZE * CELL_SIZE;
            Rectangle r = { position.x, position.y + 15, CELL_SIZE, CELL_SIZE};
            zoneMark.emplace_back(r);
            
        }
    }
    attack1state = true;
}

void Game::PrepareBossAttack2()
{
     bool horizontal = rand() % 2; // 0 = vertical, 1 = horizontal

    if (horizontal) {
        // Marcar cada 2 filas empezando por la 1
        for (int j = 1; j < 13; j += 2) {
            for (int i = 0; i < 31; ++i) {
                float x = i * CELL_SIZE;
                float y = j * CELL_SIZE + SCREEN_HEIGHT / 5;

                Rectangle r = { x, y, CELL_SIZE, CELL_SIZE };
                zoneMark.emplace_back(r);
            }
        }
    } else {
        // Marcar cada 2 columnas empezando por la 1
        for (int i = 1; i < 31; i += 2) {
            for (int j = 0; j < 13; ++j) {
                float x = i * CELL_SIZE;
                float y = j * CELL_SIZE + SCREEN_HEIGHT / 5;

                Rectangle r = { x, y, CELL_SIZE, CELL_SIZE };
                zoneMark.emplace_back(r);
            }
        }
    }


    attack1state = true;
}

void Game::PrepareBossAttack3()
{
    if (propagationState == false) {
        // Centro del círculo (usamos la posición del jugador como ejemplo)
        centerX = 1500 / CELL_SIZE;
        centerY = 400 / CELL_SIZE;

        currentRadius = radius;

        // Generamos el círculo inicial
        CreateCircle(centerX, centerY, radius);

        // Luego de crear el círculo, generamos los blasts en esa zona
        BossCoroutine(GetFrameTime());

        propagationState = true;
        attack1state = true;
    }
    

}

void Game::BossAttack1()
{
    for each (Rectangle var in zoneMark)
    {
        Vector2 v = { var.x,var.y };
        AddBlasts1(v);
    }
    
    zoneMark.clear();

}

void Game::CreateCircle(int centerX, int centerY, int radius)
{

    // Iteramos sobre el mapa para marcar las celdas dentro del círculo
    for (int i = 0; i < 31; ++i) {  // Columnas
        for (int j = 0; j < 13; ++j) {  // Filas
            // Calculamos la distancia de cada celda al centro
            int dx = i - centerX;
            int dy = j - centerY;

            // Si la celda está dentro del círculo (distancia al centro <= radio)
            float distanceSquared = dx * dx + dy * dy;
            float radiusSquared = radius * radius;
            if (distanceSquared >= radiusSquared - radius && distanceSquared <= radiusSquared + radius) {
                float x = i * CELL_SIZE;
                float y = j * CELL_SIZE + SCREEN_HEIGHT / 5;

                Rectangle r = { x, y, CELL_SIZE, CELL_SIZE };
                zoneMark.emplace_back(r);
            }
        }
    }
}

void Game::BossCoroutine(float time)
{
    
    zoneMarkCooldown -= time;
    if (zoneMarkCooldown <= 0) {
        BossAttack1();
        attack1state = false;
        zoneMarkCooldown = 2.0f;
    }
}


void Game::GameOver() {
    gameRunning = false;
    isDead = true;
    PUWP = false;
    INVINCIBLE = false;
}
