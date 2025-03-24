#include "Game.h"
#include "Constants.h"
#include <iostream>
#include "Blast.h"
#include "Ballom.h"
#include "Doria.h"
#include "PowerUp.h"
#include "FireUp.h"
#include "SpeedUp.h"
#include "Bomb.h"
#include "Exit.h"
#include "TextureManager.h"


TextureManager textureManager;
std::vector<Blast> blasts; // Definición de la variable global
std::vector<std::unique_ptr<PowerUp>> powerups;
std::vector<std::unique_ptr<Enemy>> enemies;
std::vector<Exit> exits;
int enemyCounter = 0;
int exitCounter = 0;
bool playerWalking = false;
Vector2 enemyStartPos = { INITIAL_PLAYER_X, INITIAL_PLAYER_Y};

Ballom enemy(enemyStartPos);

Game::Game() : player(INITIAL_PLAYER_X, INITIAL_PLAYER_Y), onBomb(false) {
    textureManager.LoadTextures();
    AddWalls();
}

void Game::Run() {

    
    player.SetTexture(textureManager.GetTexture(0));
    player.SetDirection({1,0});
    

    while (!WindowShouldClose()) {
        Update();
        Draw();
    }
    
    CloseWindow();
}

void Game::Update() {
    int prevX = player.GetX();
    int prevY = player.GetY();
    onBomb = CheckCollisions(player.GetBounds()) == 3;
    playerWalking = false;

    if (IsKeyDown(KEY_RIGHT)) { 
        Vector2 v = { 1,0 };
        player.Move(PLAYER_SPEED, 0, v);
        playerWalking = true;
        if (CheckCollisions(player.GetBounds()) != 0) {
            player.SetX(prevX);
        }else if (player.GetX() > CAMERA_BORDER_MIN_X && player.GetX() < CAMERA_BORDER_MAX_X) {
            //MoverDerecha
            CAMERA_OFFSET_X -= PLAYER_SPEED;
            
        }
        CheckPowerUPCollision(player.GetBounds());
    }
    if (IsKeyDown(KEY_LEFT)) { 
        player.Move(-PLAYER_SPEED, 0, {-1, 0});
        playerWalking = true;
        if (CheckCollisions(player.GetBounds()) != 0) {
            player.SetX(prevX);
        }else if (player.GetX() > CAMERA_BORDER_MIN_X && player.GetX() < CAMERA_BORDER_MAX_X) {
            //MoverIzquierda
            
            CAMERA_OFFSET_X += PLAYER_SPEED;
        }
        CheckPowerUPCollision(player.GetBounds());
    }
    if (IsKeyDown(KEY_DOWN)) { 
        player.Move(0, PLAYER_SPEED, {0,-1});
        playerWalking = true;
        if (CheckCollisions(player.GetBounds()) != 0) {
            player.SetY(prevY);
        }
        else if (player.GetY() > CAMERA_BORDER_MIN_Y && player.GetY() < CAMERA_BORDER_MAX_Y) {
            //MoverIzquierda
            CAMERA_OFFSET_Y -= PLAYER_SPEED;
        }
        CheckPowerUPCollision(player.GetBounds());
    }
    if (IsKeyDown(KEY_UP)) { 
        player.Move(0, -PLAYER_SPEED, {0,1});
        playerWalking = true;
        if (CheckCollisions(player.GetBounds()) != 0) {
            player.SetY(prevY);
        }
        else if (player.GetY() > CAMERA_BORDER_MIN_Y && player.GetY() < CAMERA_BORDER_MAX_Y) {
            //MoverIzquierda
            CAMERA_OFFSET_Y += PLAYER_SPEED;
        }
        CheckPowerUPCollision(player.GetBounds());
    }
    Vector2 playerPos = {player.GetX(), player.GetY()};
    if (CheckBlastDamage(playerPos)) {
        if (player.GetLife() <= 0) {
            GameOver();
        }
        else {
            //ResetStage();
        }
    }
    if (playerWalking) {
        player.SetIdle(false);
    }
    else {
        player.SetIdle(true);
    }
    player.Update();

    if (IsKeyPressed(KEY_SPACE)) AddBomb();
    //if (IsKeyPressed(KEY_E)) {
        //Player player2();
    //}

    for (auto& blast : blasts) {
        blast.Update(GetFrameTime());
        if (!blast.active) {
            //blast.UnloadT(); 
        }
    }
    
    blasts.erase(std::remove_if(blasts.begin(), blasts.end(),
        [](Blast& b) { return !b.active; }), blasts.end());

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
    for (auto it = enemies.begin(); it != enemies.end(); /* no incremento aquí */) {
        if (blasts.size() > 0) {
            if ((*it)->IsDead() == false) {
                if (CheckBlastDamage((*it)->GetPosition())) {
                    (*it)->Die(); // Llama a Die() en lugar de eliminar inmediatamente
                }
            }
        }
        if (!(*it)->IsActive()) {
            it = enemies.erase(it); // Elimina el enemigo solo si ya no está activo
            enemyCounter--;
        }
        else {
            (*it)->Update(GetFrameTime(), walls, softBlocks); // Actualiza si todavía está activo
            ++it; // Incrementa el iterador manualmente
        }
    }
    for (auto it = softBlocks.begin(); it != softBlocks.end(); /* no incremento aquí */) {
        if (it->IsDestroyed()) {
            it->Update();
            if (!it->IsActive()) {
                it = softBlocks.erase(it);  // `erase` devuelve el siguiente iterador válido
                continue; // Continuar sin incrementar el iterador manualmente
            }
        }
        ++it; // Incremento explícito del iterador
    }

}

void Game::Draw() {
    BeginDrawing();
    ClearBackground(DARKGREEN);

    for (auto& wall : walls) wall.Draw();
    for (auto& exit : exits) exit.Draw();
    for (auto& softBlock : softBlocks) softBlock.Draw();
    for (const auto& powerup : powerups) {
        powerup->Draw(); // Accede al objeto apuntado por unique_ptr
    }
    for (auto& bomb : bombs) bomb->Draw();
    for (auto& blast : blasts) {
        blast.Draw();
    }
    for (const auto& enemy : enemies) {
        enemy->Draw(); // Accede al objeto apuntado por unique_ptr
    }
    

    player.Draw();
    DrawText(TextFormat("Direccion: %d, %d", enemy.GetDirection().x, enemy.GetDirection().y), 100, 90, 40, WHITE);
    DrawText(TextFormat("Vidas: %d", PLAYER_SPEED), 100, 50, 40, WHITE);
    DrawText(TextFormat("Player Pos: %d,%d", player.GetX(), player.GetY()), 600, 50, 40, WHITE);
    DrawText(TextFormat("Blasts: %d", blasts.size()), 1200, 50, 40, WHITE);
    DrawFPS(900,150);
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
                    walls.back().SetTexture(textureManager.GetTexture(5));
                }
                else if ((i <= 3 && j <= 3)) {
                    //Lugar SEguro
                }
                else{
                    if (GetRandomValue(1, 4) == 1){
                    softBlocks.emplace_back(i * CELL_SIZE, j * CELL_SIZE + SCREEN_HEIGHT / 5);
                    softBlocks.back().SetTexture(textureManager.GetTexture(5));
                        if (GetRandomValue(1, 4) == 1) {
                            Vector2 v = { i * CELL_SIZE, j * CELL_SIZE + SCREEN_HEIGHT / 5 };
                            AddEnemy(v);
                        }
                    }
                }
            }
            
            else {
                walls.emplace_back(i * CELL_SIZE, j * CELL_SIZE + SCREEN_HEIGHT / 5);
                walls.back().SetTexture(textureManager.GetTexture(5));
            }
        }
        
    }
    int temp = GetRandomValue(0, softBlocks.size() - 1);
    exits.emplace_back(softBlocks.at(temp).GetBound().x, softBlocks.at(temp).GetBound().y);
    exits.back().SetTexture(textureManager.GetTexture(7));
    printf("Salida en: %f, %f", exits.back().GetBound().x, exits.back().GetBound().y);
}

void Game::AddBomb() {
    
    if (bombs.size() < 10) {
        

        bombs.push_back(new Bomb(((player.GetX() / CELL_SIZE) * CELL_SIZE), ((player.GetY() / CELL_SIZE) * CELL_SIZE) + 15));
        bombs.back()->SetTexture(textureManager.GetTexture(2));
        
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
    blasts.back().SetTexture(textureManager.GetTexture(1));
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
            blasts.back().SetTexture(textureManager.GetTexture(1));
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
                powerups.push_back(std::make_unique<SpeedUp>((*it).GetBound().x, (*it).GetBound().y));
                powerups.back()->SetTexture(textureManager.GetTexture(6));
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
        enemies.back()->SetTexture(textureManager.GetTexture(4));
    }
    else {
        enemies.push_back(std::make_unique<Doria>(pos)); // Crear y agregar Doria
        enemies.back()->SetTexture(textureManager.GetTexture(3));
        enemies.back()->SetSpeed(0.5f);
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

void Game::CheckPowerUPCollision(Rectangle rec) {
    for (auto it = powerups.begin(); it != powerups.end();) {
        if (CheckCollisionRecs(rec, (*it)->GetBound())) {
            (*it)->Effect(); // Aplicar el efecto

            // Eliminar el PowerUp del vector después de aplicar el efecto
            it = powerups.erase(it); // Esto elimina el elemento y avanza al siguiente
        }
        else {
            ++it; // Solo avanza al siguiente si no se eliminó el actual
        }
    }
}

bool Game::CheckBlastDamage(Vector2 pos) {
    Rectangle rec = { pos.x, pos.y, player.GetBounds().x, player.GetBounds().y };
    for (const auto& blast : blasts) {
        Rectangle r = {blast.position.x, blast.position.y, CELL_SIZE - 20, CELL_SIZE - 20};
        if (CheckCollisionRecs(rec, r)) { 
            //printf("Te ha dao en: %f,%f y player: %f, %f\n", blast.position.x, blast.position.y, rec.x,rec.y);
            //printf("El rectangulo esta en: %f,%f y mide: %d,%d", blast.position.x,blast.position.y, CELL_SIZE - 20, CELL_SIZE - 20);
            return true; 
        }
    }
    return false;
}

void Game::ResetStage() {
    blasts.clear();
    bombs.clear();
    enemies.clear();
    enemyCounter = 0;
    CAMERA_OFFSET_X = 0;
    CAMERA_OFFSET_Y = 0;
    player.SetPosition(INITIAL_PLAYER_X, INITIAL_PLAYER_Y);
    player.DecreaseLife();
    AddWalls();
}
void Game::GameOver() {

}
