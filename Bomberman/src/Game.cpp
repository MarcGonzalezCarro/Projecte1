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
#include "SaveGame.h"
#include "ResourceManager.h"
#include <sstream>
#include <string>
#include <algorithm>
#include <raymath.h>



ResourceManager resourceManager;
Texture2D initialScreen;
Texture2D arrowTexture;

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

int currentStage = 5;

std::vector<ScoreEntry> entries = SaveGame::GetEntriesFromFile();
std::vector<Blast> blasts; 
std::vector<std::unique_ptr<PowerUp>> powerups;
std::vector<std::unique_ptr<Enemy>> enemies;
std::vector<Exit> exits;

std::unique_ptr<Boss> currentBoss;
float bossAttackCooldown = 5.0f;
float zoneMarkCooldown = 2.0f;
bool attack1state = false;
float propagationTime;
float expansionSpeed = 2.0f;
int currentRadius = 3;
int radius = 3;
float maxRadius;
int centerX, centerY;
bool propagationState;
std::vector<Rectangle> zoneMark;

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
double remainingTime;

Color semiTransparentRed = Color{ 255, 0, 0, 80};

////////////////////////////
//FOR STATS
////////////////////////////
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
////////////////////////////

Game::Game() : player(INITIAL_PLAYER_X, INITIAL_PLAYER_Y), player2(INITIAL_PLAYER_X,INITIAL_PLAYER_Y), onBomb(false) {
    InitAudioDevice();
    resourceManager.LoadTextures();
    resourceManager.LoadMusic();
    resourceManager.LoadSounds();
    initialScreen = resourceManager.GetTexture(15);
    arrowTexture = resourceManager.GetTexture(16);
    menuSong = resourceManager.GetMusic(0);
    gameSong = resourceManager.GetMusic(1);
    specialGameSong = resourceManager.GetMusic(2);
    startTime = GetTime();
    targetTime = 200.0;
    
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
                SaveGame::SaveNameToFile(playerName, playerScore, ballomKills, onilKills, dahlKills, minvoKills, doriaKills, ovapeKills, passKills, pontanKills, bombsPlanted,softBlocksDestroyed,powerUpsPicked,fireUpCounter,bombUpCounter,speedUpCounter);
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
            if (INVINCIBLE == false) {
                player.Die();
            }
        }
    }

    if (CheckBlastDamage(playerPos)) {
        if(!player.IsDead()){
            PlaySound(resourceManager.GetSound(4));
            if (INVINCIBLE == false) {
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
        PlaySound(resourceManager.GetSound(2));
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

        if (CheckBlastDamage(playerPos2)) {
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
        playerScore += 100000;
    }
    if (IsKeyDown(KEY_B)) {
        PrepareBossAttack1();
    }
    
    //printf("%d", PUWP);
    //////////////////////////////////////////////////////////////////////////////////////////////////////////
    for (auto it = blasts.begin(); it != blasts.end(); ) {
        it->Update(GetFrameTime()); 
        if (!it->active) {
            it = blasts.erase(it); // Eliminar blast si no está activo
        }
        else {
            ++it; 
        }
    }

    for (auto& powerup : powerups) {
        powerup->Update();
    }

    for (auto it = bombs.begin(); it != bombs.end(); ) {
        float temp = (*it)->Update(GetFrameTime());
        
        if (temp <= 0) {
            PlaySound(resourceManager.GetSound(6));
            AddBlasts(**it);
            delete* it;           
            it = bombs.erase(it); 
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
                ballomKills++;
                break;
            case 1:
                onilKills++;
                break;
            case 2:
                dahlKills++;
                break;
            case 3:
                minvoKills++;
                break;
            case 4:
                doriaKills++;
                break;
            case 5:
                ovapeKills++;
                break;
            case 6:
                passKills++;
                break;
            case 7:
                pontanKills++;
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
    
    currentBoss->Update(GetFrameTime(), walls, softBlocks);

    bossAttackCooldown -= GetFrameTime();
    if (bossAttackCooldown <= 0) {
        zoneMark.clear();
        PrepareBossAttack3();
        currentBoss->isAttacking = true;
        bossAttackCooldown = 3.0f;
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
            AddBlasts1({(float) centerX, (float)centerY });
            BossAttack1();

            // Si el radio ha alcanzado el máximo, terminamos la propagación
            if (currentRadius >= maxRadius) {
                propagationState = false;
            }
        }
    }

    elapsedTime = GetTime() - startTime;
    remainingTime = targetTime - elapsedTime;
}

void Game::Draw() {
    BeginDrawing();
   
    if (startScreen) {
        ClearBackground(BLACK);
        DrawTextureEx(initialScreen, {(float)SCREEN_WIDTH/2 - 650, 50},0,4, WHITE);
        DrawTextureEx(arrowTexture, { arrowPositions[menuChoice].x, arrowPositions[menuChoice].y}, 0, 4, WHITE);
    }
    else if(gameRunning){

        // Cámara 1
        Camera2D camera1 = { 0 };
        Camera2D camera2 = { 0 };
        Vector2 target1 = { player.GetBounds().x, SCREEN_HEIGHT/2 + 200};

        if (!isCoop) {
            camera1.zoom = 0.7f;

            target1.x = Clamp(target1.x, 700, 1025);

            camera1.target = target1;
            camera1.offset = { SCREEN_WIDTH / 4.0f, SCREEN_HEIGHT / 2.0f };

            BeginMode2D(camera1);
            ClearBackground(DARKGREEN);
            DrawRectangle(0, 0, 4000, 250, GRAY);
            for (auto& wall : walls) wall.Draw();
            for (auto& exit : exits) exit.Draw();
            for (auto& softBlock : softBlocks) softBlock.Draw();
            for (const auto& powerup : powerups) powerup->Draw();
            for (auto& bomb : bombs) bomb->Draw();
            for (auto& blast : blasts) blast.Draw();
            for (const auto& enemy : enemies) enemy->Draw();
            if (player.IsActive()) player.Draw();
            if (currentStage == 5) {
                currentBoss->Draw();
                for each (Rectangle var in zoneMark)
                {
                    DrawRectangle(var.x,var.y,var.width,var.height,semiTransparentRed);
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
        ClearBackground(DARKGREEN);
        DrawRectangle((0) , (0), 4000, 250 , GRAY);
        for (auto& wall : walls) wall.Draw();
        for (auto& exit : exits) exit.Draw();
        for (auto& softBlock : softBlocks) softBlock.Draw();
        for (const auto& powerup : powerups) powerup->Draw();
        for (auto& bomb : bombs) bomb->Draw();
        for (auto& blast : blasts) blast.Draw();
        for (const auto& enemy : enemies) enemy->Draw();
        if (player.IsActive()) player.Draw();
        if (player2.IsActive()) player2.Draw();
        EndMode2D();
        EndScissorMode();

        // Dibujo con cámara 2 (jugador 2)
        
            BeginScissorMode(SCREEN_WIDTH / 2, 0, SCREEN_WIDTH / 2, SCREEN_HEIGHT);  // Delimita lado derecho
            BeginMode2D(camera2);
            ClearBackground(DARKGREEN);
            DrawRectangle(0, 0, 4000, 250 , GRAY);
            for (auto& wall : walls) wall.Draw();
            for (auto& exit : exits) exit.Draw();
            for (auto& softBlock : softBlocks) softBlock.Draw();
            for (const auto& powerup : powerups) powerup->Draw();
            for (auto& bomb : bombs) bomb->Draw();
            for (auto& blast : blasts) blast.Draw();
            for (const auto& enemy : enemies) enemy->Draw();
            if (player.IsActive()) player.Draw();
            if (player2.IsActive()) player2.Draw();
            EndMode2D();
            EndScissorMode();
        }
        if (isCoop) {
            DrawRectangle(SCREEN_WIDTH / 2 - 2, 0, 4, SCREEN_HEIGHT, BLACK);
        }
        // HUD común (fuera de las cámaras)
        DrawText(TextFormat("TIME %.0f", remainingTime), 30, 20, 40, WHITE);
        DrawText(TextFormat("SCORE %d", playerScore), SCREEN_WIDTH / 2 - 100, 20, 40, WHITE);
        DrawText(TextFormat("LEFT %d", player.GetLife()), SCREEN_WIDTH - 200, 20, 40, WHITE);
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
            DrawText(("Name: " + selectedEntry.name).c_str(), 800, 100, 30, WHITE);
            DrawText(("Score: " + std::to_string(selectedEntry.score)).c_str(), 800, 150, 30, WHITE);

            // Mostrar las estadísticas del jugador
            DrawText(("Ballom Kills: " + std::to_string(selectedEntry.ballomKills)).c_str(), 800, 200, 30, WHITE);
            DrawText(("Onil Kills: " + std::to_string(selectedEntry.onilKills)).c_str(), 800, 250, 30, WHITE);
            DrawText(("Dahl Kills: " + std::to_string(selectedEntry.dahlKills)).c_str(), 800, 300, 30, WHITE);
            DrawText(("Minvo Kills: " + std::to_string(selectedEntry.minvoKills)).c_str(), 800, 350, 30, WHITE);
            DrawText(("Doria Kills: " + std::to_string(selectedEntry.doriaKills)).c_str(), 800, 400, 30, WHITE);
            DrawText(("Ovape Kills: " + std::to_string(selectedEntry.ovapeKills)).c_str(), 800, 450, 30, WHITE);
            DrawText(("Pass Kills: " + std::to_string(selectedEntry.passKills)).c_str(), 800, 500, 30, WHITE);
            DrawText(("Pontan Kills: " + std::to_string(selectedEntry.pontanKills)).c_str(), 800, 550, 30, WHITE);
            DrawText(("Bombs Planted: " + std::to_string(selectedEntry.bombsPlanted)).c_str(), 800, 600, 30, WHITE);
            DrawText(("Soft Blocks Destroyed: " + std::to_string(selectedEntry.softBlocksDestroyed)).c_str(), 800, 650, 30, WHITE);
            DrawText(("Power-Ups Picked: " + std::to_string(selectedEntry.powerUpsPicked)).c_str(), 800, 700, 30, WHITE);
            DrawText(("Fire-Up Counter: " + std::to_string(selectedEntry.fireUpCounter)).c_str(), 850, 750, 30, WHITE);
            DrawText(("Bomb-Up Counter: " + std::to_string(selectedEntry.bombUpCounter)).c_str(), 850, 800, 30, WHITE);
            DrawText(("Speed-Up Counter: " + std::to_string(selectedEntry.speedUpCounter)).c_str(), 850, 850, 30, WHITE);

            // Volver al menú principal o salir
            DrawText("Back", 140, 700, 40, WHITE);
            if (viewingScores) {
                DrawTextureEx(arrowTexture, { 100, 700 }, 0, 4, WHITE);
            }
        }
    }
    else if (isDead) {
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
    }
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
                            if (GetRandomValue(1, 4) == 1) {
                                Vector2 v = { i * CELL_SIZE, j * CELL_SIZE + SCREEN_HEIGHT / 5 };
                                AddEnemy(v);
                            }
                            else {
                                softBlocks.emplace_back(i * CELL_SIZE, j * CELL_SIZE + SCREEN_HEIGHT / 5);
                                softBlocks.back().SetTexture(resourceManager.GetTexture(12));
                            }
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
        printf("Salida en: %f, %f", exits.back().GetBound().x, exits.back().GetBound().y);
    }
    else {
        for (int i = 0; i < 31; i++) {
            for (int j = 0; j < 13; j++) {
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
    
    if (bombs.size() < MAX_BOMBS) {
        

        bombs.push_back(new Bomb((((int)(x) / CELL_SIZE) * CELL_SIZE), (((int)(y)/ CELL_SIZE) * CELL_SIZE) + 15));
        bombs.back()->SetTexture(resourceManager.GetTexture(3));

        bombsPlanted++;
        
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
    blasts.back().direction = 0;

}

bool Game::IsBlastBlocked(Vector2 position) {
    for (const auto& wall : walls) {
        if (CheckCollisionRecs({ position.x, position.y + 1, (float) CELL_SIZE, (float)CELL_SIZE}, wall.GetBound())) {
            
            return true;  // Blockeado 
            
        }
    }
    for (auto it = softBlocks.begin(); it != softBlocks.end(); ++it) {
        if (CheckCollisionRecs({ position.x, position.y + 1, (float)CELL_SIZE, (float)CELL_SIZE }, it->GetBound())) {
            
            if (it->GetBound().x != exits.back().GetBound().x || it->GetBound().y != exits.back().GetBound().y) {
                int ran = std::rand() % 20;
                if (ran == 1) {
                    powerups.push_back(std::make_unique<SpeedUp>((*it).GetBound().x, (*it).GetBound().y));
                    powerups.back()->SetTexture(resourceManager.GetTexture(13));
                }
                if (ran == 2) {
                    powerups.push_back(std::make_unique<BombUp>((*it).GetBound().x, (*it).GetBound().y));
                    powerups.back()->SetTexture(resourceManager.GetTexture(13));
                }
                if (ran == 3) {
                    powerups.push_back(std::make_unique<FireUp>((*it).GetBound().x, (*it).GetBound().y));
                    powerups.back()->SetTexture(resourceManager.GetTexture(13));
                }
                if (ran == 4) {
                    powerups.push_back(std::make_unique<WallPass>((*it).GetBound().x, (*it).GetBound().y));
                    powerups.back()->SetTexture(resourceManager.GetTexture(13));
                }
                if (ran == 5) {
                    powerups.push_back(std::make_unique<BombPass>((*it).GetBound().x, (*it).GetBound().y));
                    powerups.back()->SetTexture(resourceManager.GetTexture(13));
                }
                if (ran == 6) {
                    powerups.push_back(std::make_unique<FlamePass>((*it).GetBound().x, (*it).GetBound().y));
                    powerups.back()->SetTexture(resourceManager.GetTexture(13));
                }
                if (ran == 7) {
                    powerups.push_back(std::make_unique<Invincible>((*it).GetBound().x, (*it).GetBound().y));
                    powerups.back()->SetTexture(resourceManager.GetTexture(13));
                }
                if (ran == 8) {
                    powerups.push_back(std::make_unique<RemoteControl>((*it).GetBound().x, (*it).GetBound().y));
                    powerups.back()->SetTexture(resourceManager.GetTexture(13));
                }
                else
                {
                    powerups.push_back(std::make_unique<Invincible>((*it).GetBound().x, (*it).GetBound().y));
                    powerups.back()->SetTexture(resourceManager.GetTexture(13));
                }
            }
            
            //printf("Añadido en: %f, %f\n", (*it).GetBound().x, (*it).GetBound().y);
            it->Destroy();
            return true;  // Bloqueado
        }
    }
    return false;  // No Blockeado :D
}

void Game::AddEnemy(Vector2 pos) {
    int ran = std::rand() % 20;
    if (ran == 0) {
        enemies.push_back(std::make_unique<Ballom>(pos)); // Crear y agregar Ballom
        enemies.back()->SetTexture(resourceManager.GetTexture(4));
    }
    else if (ran == 1) {
        enemies.push_back(std::make_unique<Onil>(pos)); // Crear y agregar Onil
        enemies.back()->SetTexture(resourceManager.GetTexture(5));
        enemies.back()->SetSpeed(1.3f);
    }
    else if (ran == 2) {
        enemies.push_back(std::make_unique<Dahl>(pos)); // Crear y agregar Dahl
        enemies.back()->SetTexture(resourceManager.GetTexture(6));
        enemies.back()->SetSpeed(1.3f);
    }
    else if (ran == 3) {
        enemies.push_back(std::make_unique<Minvo>(pos)); // Crear y agregar Minvo
        enemies.back()->SetTexture(resourceManager.GetTexture(7));
        enemies.back()->SetSpeed(1.5f);
    }
    else if (ran == 4) {
        enemies.push_back(std::make_unique<Doria>(pos)); // Crear y agregar Doria
        enemies.back()->SetTexture(resourceManager.GetTexture(8));
        enemies.back()->SetSpeed(0.6f);
    }
    else if (ran == 5) {
        enemies.push_back(std::make_unique<Ovape>(pos)); // Crear y agregar Ovape
        enemies.back()->SetTexture(resourceManager.GetTexture(9));
    }
    else if (ran == 6) {
        enemies.push_back(std::make_unique<Pass>(pos)); // Crear y agregar Pass
        enemies.back()->SetTexture(resourceManager.GetTexture(10));
        enemies.back()->SetSpeed(1.8f);
    }
    else{
        enemies.push_back(std::make_unique<Pontan>(pos)); // Crear y agregar Pontan
        enemies.back()->SetTexture(resourceManager.GetTexture(11));
        enemies.back()->SetSpeed(2.0f);
    }
    enemyCounter++;
}

void Game::AddBoss(Vector2 pos)
{
    currentBoss = std::make_unique<MechaBlastX>(pos);
    currentBoss->SetTexture(resourceManager.GetTexture(17));

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
            //printf("El rectangulo esta en: %f,%f y mide: %d,%d", blast.position.x,blast.position.y, CELL_SIZE - 20, CELL_SIZE - 20);
            return true; 
        }
    }
    return false;
}

void Game::ResetStage() {
    gameRunning = false;
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
    // Centro del círculo (usamos la posición del jugador como ejemplo)
    centerX = 1500 / CELL_SIZE;
    centerY = 400 / CELL_SIZE;


    // Generamos el círculo inicial
    CreateCircle(centerX, centerY, radius);

    // Luego de crear el círculo, generamos los blasts en esa zona
    BossAttack1();

    // Propagación de los blasts (se expande después de cierto tiempo o de forma gradual)
    StartBlastPropagation(centerX, centerY, radius);

}

void Game::BossAttack1()
{
    for each (Rectangle var in zoneMark)
    {
        Vector2 v = { var.x,var.y };
        AddBlasts1(v);
    }
    

}

void Game::CreateCircle(int centerX, int centerY, int radius)
{
    // Limpiar las zonas marcadas anteriores
    zoneMark.clear();

    // Iteramos sobre el mapa para marcar las celdas dentro del círculo
    for (int i = 0; i < 31; ++i) {  // Columnas
        for (int j = 0; j < 13; ++j) {  // Filas
            // Calculamos la distancia de cada celda al centro
            int dx = i - centerX;
            int dy = j - centerY;

            // Si la celda está dentro del círculo (distancia al centro <= radio)
            if ((dx * dx + dy * dy) <= (radius * radius)) {
                float x = i * CELL_SIZE;
                float y = j * CELL_SIZE + SCREEN_HEIGHT / 5 + 15;

                Rectangle r = { x, y, CELL_SIZE, CELL_SIZE };
                zoneMark.emplace_back(r);
            }
        }
    }
}

void Game::StartBlastPropagation(int centerX, int centerY, int initialRadius)
{
    
    expansionSpeed = 0.01f;
    // Propagación en función del tiempo o frames
    propagationTime = 0.0f;  // Tiempo acumulado

    // Controlamos la propagación de los blasts
    propagationState = true;
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
