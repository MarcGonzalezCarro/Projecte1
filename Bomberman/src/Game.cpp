#include "Game.h"
#include "Constants.h"
#include <iostream>
#include "Blast.h"
#include "Ballom.h"
#include "Doria.h"
#include "PowerUp.h"
#include "BombUp.h"
#include "FireUp.h"
#include "SpeedUp.h"
#include "Bomb.h"
#include "Exit.h"
#include "SaveGame.h"
#include "ResourceManager.h"
#include <sstream>
#include <string>
#include <algorithm>



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

char playerName[50] = "";  // Buffer
int nameLength = 0;
int playerScore = 0;

Vector2 arrowPositions[] = {
    {565, 660}, 
    {820, 660},  
    {600, 720},
    {840, 720}
};

int currentStage = 1;

std::vector<ScoreEntry> entries = SaveGame::GetEntriesFromFile();
std::vector<Blast> blasts; 
std::vector<std::unique_ptr<PowerUp>> powerups;
std::vector<std::unique_ptr<Enemy>> enemies;
std::vector<Exit> exits;

Music menuSong;
Music gameSong;
Music specialGameSong;

int enemyCounter = 0;

bool playerWalking = false;

double startTime = GetTime();
double targetTime = 200.0;
double elapsedTime;
double remainingTime;

////////////////////////////
//FOR STATS
////////////////////////////
int ballomKills = 0;
int doriaKills = 0;
int bombsPlanted = 0;
int softBlocksDestroyed = 0;
int powerUpsPicked = 0;
int fireUpCounter = 0;
int bombUpCounter = 0;
int speedUpCounter = 0;
////////////////////////////

Game::Game() : player(INITIAL_PLAYER_X, INITIAL_PLAYER_Y), onBomb(false) {
    InitAudioDevice();
    resourceManager.LoadTextures();
    resourceManager.LoadMusic();
    initialScreen = resourceManager.GetTexture(8);
    arrowTexture = resourceManager.GetTexture(9);
    menuSong = resourceManager.GetMusic(0);
    gameSong = resourceManager.GetMusic(1);
    specialGameSong = resourceManager.GetMusic(2);
    startTime = GetTime();
    targetTime = 200.0;
    
}

void Game::Run() {
    
    player.SetTexture(resourceManager.GetTexture(0));
    player.SetDirection({ 1, 0 });

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
                SaveGame::SaveNameToFile(playerName, playerScore, ballomKills,doriaKills,bombsPlanted,softBlocksDestroyed,powerUpsPicked,fireUpCounter,bombUpCounter,speedUpCounter);
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
        ResetStage();
    }
    if (!player.IsDead()) {
        if (IsKeyDown(KEY_RIGHT)) {
            Vector2 v = { 1,0 };
            player.Move(PLAYER_SPEED, 0, v);
            playerWalking = true;
            if (CheckCollisions(player.GetBounds()) != 0) {
                player.SetX(prevX);
            }
            else if (player.GetX() > CAMERA_BORDER_MIN_X && player.GetX() < CAMERA_BORDER_MAX_X) {
                //MoverDerecha

                CAMERA_OFFSET_X -= PLAYER_SPEED;


            }

        }
        if (IsKeyDown(KEY_LEFT)) {
            player.Move(-PLAYER_SPEED, 0, { -1, 0 });
            playerWalking = true;
            if (CheckCollisions(player.GetBounds()) != 0) {
                player.SetX(prevX);
            }
            else if (player.GetX() > CAMERA_BORDER_MIN_X && player.GetX() < CAMERA_BORDER_MAX_X) {
                //MoverIzquierda

                CAMERA_OFFSET_X += PLAYER_SPEED;
            }

        }
        if (IsKeyDown(KEY_DOWN)) {
            player.Move(0, PLAYER_SPEED, { 0,-1 });
            playerWalking = true;
            if (CheckCollisions(player.GetBounds()) != 0) {
                player.SetY(prevY);
            }
            else if (player.GetY() > CAMERA_BORDER_MIN_Y && player.GetY() < CAMERA_BORDER_MAX_Y) {
                //MoverAbajo
                if (GLOBAL_SCALE > 0.7f) {
                    CAMERA_OFFSET_Y -= PLAYER_SPEED;
                }
            }

        }
        if (IsKeyDown(KEY_UP)) {
            player.Move(0, -PLAYER_SPEED, { 0,1 });
            playerWalking = true;
            if (CheckCollisions(player.GetBounds()) != 0) {
                player.SetY(prevY);
            }
            else if (player.GetY() > CAMERA_BORDER_MIN_Y && player.GetY() < CAMERA_BORDER_MAX_Y) {
                //MoverArriba
                if (GLOBAL_SCALE > 0.7f) {
                    CAMERA_OFFSET_Y += PLAYER_SPEED;
                }
            }

        }
    }
    Vector2 playerPos = {player.GetX(), player.GetY()};
    if (CheckEnemyCollision()) {
        if (!player.IsDead()) {
            player.Die();
        }
    }

    if (CheckBlastDamage(playerPos)) {
        if (player.GetLife() <= 0) {
            GameOver();
        }
        else if(!player.IsDead()){
            player.Die();
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

    if (IsKeyPressed(KEY_SPACE)) AddBomb();
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

    for (auto it = bombs.begin(); it != bombs.end(); ) {
        float temp = (*it)->Update(GetFrameTime());
        
        if (temp <= 0) {
            
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
                doriaKills++;
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
                it = softBlocks.erase(it); 
                continue;
            }
        }
        ++it; 
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
        ClearBackground(DARKGREEN);
        DrawRectangle((0 + CAMERA_OFFSET_X) * GLOBAL_SCALE,(0 + CAMERA_OFFSET_Y) * GLOBAL_SCALE,4000,250 * GLOBAL_SCALE, GRAY);
        for (auto& wall : walls) wall.Draw();
        for (auto& exit : exits) exit.Draw();
        for (auto& softBlock : softBlocks) softBlock.Draw();
        for (const auto& powerup : powerups) {
            powerup->Draw(); 
        }
        for (auto& bomb : bombs) bomb->Draw();
        for (auto& blast : blasts) {
            blast.Draw();
        }
        for (const auto& enemy : enemies) {
            enemy->Draw(); 
        }

        if (player.IsActive()) {
            player.Draw();
        }
        
        DrawText(TextFormat("TIME %.0f", remainingTime), 30, 80, 40, WHITE);
        DrawText(TextFormat("SCORE %d", playerScore), SCREEN_WIDTH/2 - 300, 80, 40, WHITE);
        DrawText(TextFormat("LEFT %d", player.GetLife()), 1400, 80, 40, WHITE);
        DrawFPS(1200, 80);
    }
    else if (inCredits) {
        ClearBackground(BLACK);
        DrawText("", 100,100,40,WHITE);
        DrawText("Back", 140, 700, 40, WHITE);
        DrawTextureEx(arrowTexture, { 100, 700 }, 0, 4, WHITE);
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
            DrawText(("Doria Kills: " + std::to_string(selectedEntry.doriaKills)).c_str(), 800, 250, 30, WHITE);
            DrawText(("Bombs Planted: " + std::to_string(selectedEntry.bombsPlanted)).c_str(), 800, 300, 30, WHITE);
            DrawText(("Soft Blocks Destroyed: " + std::to_string(selectedEntry.softBlocksDestroyed)).c_str(), 800, 350, 30, WHITE);
            DrawText(("Power-Ups Picked: " + std::to_string(selectedEntry.powerUpsPicked)).c_str(), 800, 400, 30, WHITE);
            DrawText(("Fire-Up Counter: " + std::to_string(selectedEntry.fireUpCounter)).c_str(), 850, 450, 30, WHITE);
            DrawText(("Bomb-Up Counter: " + std::to_string(selectedEntry.bombUpCounter)).c_str(), 850, 500, 30, WHITE);
            DrawText(("Speed-Up Counter: " + std::to_string(selectedEntry.speedUpCounter)).c_str(), 850, 550, 30, WHITE);

            // Volver al menú principal o salir
            DrawText("Back", 140, 700, 40, WHITE);
            if (viewingScores) {
                DrawTextureEx(arrowTexture, { 100, 700 }, 0, 4, WHITE);
            }
        }
    }
    else if (isDead) {
        ClearBackground(BLACK);
        DrawText("Introduce Name:", (float)SCREEN_WIDTH / 2 - 200, 100, 40, WHITE);

        // Dibujar la línea de guiones bajos y reemplazarlos por las letras
        for (int i = 0; i < 5; i++) // Mostrar un máximo de 20 caracteres
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

    for (int i = 0; i < 31; i++) {
        for (int j = 0; j < 13; j++) {
            if (i != 0 && j != 0 && i != 31 - 1 && j != 13 - 1) {
                if (i % 2 == 0 && j % 2 == 0) {
                    walls.emplace_back(i * CELL_SIZE, j * CELL_SIZE + SCREEN_HEIGHT / 5);
                    walls.back().SetTexture(resourceManager.GetTexture(5));
                }
                else if ((i <= 3 && j <= 3)) {
                    //Lugar SEguro
                }
                else{
                    if (GetRandomValue(1, 4) == 1){
                    softBlocks.emplace_back(i * CELL_SIZE, j * CELL_SIZE + SCREEN_HEIGHT / 5);
                    softBlocks.back().SetTexture(resourceManager.GetTexture(5));
                        if (GetRandomValue(1, 4) == 1) {
                            Vector2 v = { i * CELL_SIZE, j * CELL_SIZE + SCREEN_HEIGHT / 5 };
                            AddEnemy(v);
                        }
                    }
                }
            }
            
            else {
                walls.emplace_back(i * CELL_SIZE, j * CELL_SIZE + SCREEN_HEIGHT / 5);
                walls.back().SetTexture(resourceManager.GetTexture(5));
            }
        }
        
    }
    int temp = GetRandomValue(0, softBlocks.size() - 1);
    exits.emplace_back(softBlocks.at(temp).GetBound().x, softBlocks.at(temp).GetBound().y);
    exits.back().SetTexture(resourceManager.GetTexture(7));
    printf("Salida en: %f, %f", exits.back().GetBound().x, exits.back().GetBound().y);
}

void Game::AddBomb() {
    
    if (bombs.size() < MAX_BOMBS) {
        

        bombs.push_back(new Bomb(((player.GetX() / CELL_SIZE) * CELL_SIZE), ((player.GetY() / CELL_SIZE) * CELL_SIZE) + 15));
        bombs.back()->SetTexture(resourceManager.GetTexture(2));

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
    blasts.back().SetTexture(resourceManager.GetTexture(1));
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
            blasts.back().SetTexture(resourceManager.GetTexture(1));
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
bool Game::IsBlastBlocked(Vector2 position) {
    for (const auto& wall : walls) {
        if (CheckCollisionRecs({ position.x, position.y + 1, (float) CELL_SIZE, (float)CELL_SIZE}, wall.GetBound())) {
            
            return true;  // Blockeado 
            
        }
    }
    for (auto it = softBlocks.begin(); it != softBlocks.end(); ++it) {
        if (CheckCollisionRecs({ position.x, position.y + 1, (float)CELL_SIZE, (float)CELL_SIZE }, it->GetBound())) {
            
            if (it->GetBound().x != exits.back().GetBound().x || it->GetBound().y != exits.back().GetBound().y) {
                int ran = std::rand() % 4;
                if (ran == 1) {
                    powerups.push_back(std::make_unique<SpeedUp>((*it).GetBound().x, (*it).GetBound().y));
                    powerups.back()->SetTexture(resourceManager.GetTexture(6));
                }
                if (ran == 2) {
                    powerups.push_back(std::make_unique<BombUp>((*it).GetBound().x, (*it).GetBound().y));
                    powerups.back()->SetTexture(resourceManager.GetTexture(6));
                }
                if (ran == 3) {
                    powerups.push_back(std::make_unique<FireUp>((*it).GetBound().x, (*it).GetBound().y));
                    powerups.back()->SetTexture(resourceManager.GetTexture(6));
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
    int ran = std::rand() % 2;
    if (ran == 0) {
        enemies.push_back(std::make_unique<Ballom>(pos)); // Crear y agregar Ballom
        enemies.back()->SetTexture(resourceManager.GetTexture(4));
    }
    else {
        enemies.push_back(std::make_unique<Doria>(pos)); // Crear y agregar Doria
        enemies.back()->SetTexture(resourceManager.GetTexture(3));
        enemies.back()->SetSpeed(0.6f);
    }
    enemyCounter++;
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

void Game::CheckExitCollision() {
    Rectangle p = {player.GetBounds().x, player.GetBounds().y, player.GetBounds().width, player.GetBounds().height};
    Rectangle e = { exits.at(0).GetBound().x, exits.at(0).GetBound().y, CELL_SIZE - 20, CELL_SIZE - 20};
    if (CheckCollisionRecs(p, e)) {
        NextLevel();
    }
}

bool Game::CheckEnemyCollision() {
    Rectangle p = { player.GetBounds().x, player.GetBounds().y, player.GetBounds().width, player.GetBounds().height };

    for (const auto& enemy : enemies) {
        Rectangle e = { enemy->GetPosition().x, enemy->GetPosition().y, CELL_SIZE - 20, CELL_SIZE - 20};
        if (CheckCollisionRecs(p, e)) return true;
    }
    return false;
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
    blasts.clear();
    bombs.clear();
    enemies.clear();
    exits.clear();
    powerups.clear();
    enemyCounter = 0;
    CAMERA_OFFSET_X = 0;
    CAMERA_OFFSET_Y = 0;
    player.SetPosition(INITIAL_PLAYER_X, INITIAL_PLAYER_Y);
    player.DecreaseLife();
    player.SetIsDead(false);
    player.SetActive(true);
    AddWalls();
    //Empezar subrutina
    ShowStageScreen("Stage ");
}

void Game::ShowStageScreen(const char* stageText) {
    // Establecer el tiempo objetivo: 2 segundos
    const float duration = 2.0f; // Duración en segundos
    float timer = 0; // Temporizador para contar el tiempo transcurrido

    while (timer < duration) {   // Mientras el tiempo transcurrido sea menor que 2 segundos
        BeginDrawing();
        ClearBackground(BLACK);

        int screenWidth = GetScreenWidth();
        int screenHeight = GetScreenHeight();
        int textWidth = MeasureText(stageText, 50); // Mide el ancho del texto
        int textX = 750;
        int textY = screenHeight / 2;

        if (timer > 0.2f && timer < 1.8f) {
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

    startTime = GetTime();
    targetTime = 200.0f;

    blasts.clear();
    bombs.clear();
    enemies.clear();
    exits.clear();
    powerups.clear();
    enemyCounter = 0;
    CAMERA_OFFSET_X = 0;
    CAMERA_OFFSET_Y = 0;
    player.SetPosition(INITIAL_PLAYER_X, INITIAL_PLAYER_Y);
    AddWalls();
    //Empezar subrutina
    ShowStageScreen("Stage ");
}

void Game::GameOver() {
    gameRunning = false;
    isDead = true;
}
