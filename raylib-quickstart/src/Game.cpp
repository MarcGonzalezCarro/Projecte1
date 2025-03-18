#include "Game.h"
#include "Constants.h"
#include <iostream>
#include "Blast.h"
#include "Ballom.h"
#include "Doria.h"
#include "TextureManager.h"


TextureManager textureManager;
std::vector<Blast> blasts; // Definición de la variable global
std::vector<std::unique_ptr<Enemy>> enemies;
int enemyCounter = 0;
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

    if (IsKeyDown(KEY_RIGHT)) { 
        Vector2 v = { 1,0 };
        player.Move(4, 0, v);
        if (CheckCollisions(player.GetBounds()) != 0) {
            player.SetX(prevX);
        }else if (player.GetX() > CAMERA_BORDER_MIN_X && player.GetX() < CAMERA_BORDER_MAX_X) {
            //MoverDerecha
            CAMERA_OFFSET_X -= 4;
            
        }
        
    }
    if (IsKeyDown(KEY_LEFT)) { 
        player.Move(-4, 0, {-1, 0});
        
        if (CheckCollisions(player.GetBounds()) != 0) {
            player.SetX(prevX);
        }else if (player.GetX() > CAMERA_BORDER_MIN_X && player.GetX() < CAMERA_BORDER_MAX_X) {
            //MoverIzquierda
            
            CAMERA_OFFSET_X += 4;
        }
        
    }
    if (IsKeyDown(KEY_DOWN)) { 
        player.Move(0, 4, {0,-1});
        
        if (CheckCollisions(player.GetBounds()) != 0) {
            player.SetY(prevY);
        }
        else if (player.GetY() > CAMERA_BORDER_MIN_Y && player.GetY() < CAMERA_BORDER_MAX_Y) {
            //MoverIzquierda
            CAMERA_OFFSET_Y -= 4;
        }
    }
    if (IsKeyDown(KEY_UP)) { 
        player.Move(0, -4, {0,1});
        
        if (CheckCollisions(player.GetBounds()) != 0) {
            player.SetY(prevY);
        }
        else if (player.GetY() > CAMERA_BORDER_MIN_Y && player.GetY() < CAMERA_BORDER_MAX_Y) {
            //MoverIzquierda
            CAMERA_OFFSET_Y += 4;
        }
    }
    Vector2 playerPos = {player.GetX() + 20, player.GetY() + 20};
    if (CheckBlastDamage(playerPos)) {
        if (player.GetLife() <= 0) {
            GameOver();
        }
        else {
            ResetStage();
        }
    }
    
    player.Update();

    if (IsKeyPressed(KEY_SPACE)) AddBomb();
    //if (IsKeyPressed(KEY_E)) AddEnemy();

    for (auto& blast : blasts) {
        blast.Update(GetFrameTime());
        if (!blast.active) {
            blast.UnloadT(); 
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
    for (auto it = enemies.begin(); it != enemies.end(); ) {
        if (CheckBlastDamage((*it)->GetPosition())) {
            it = enemies.erase(it); // std::unique_ptr se encarga de liberar la memoria
            enemyCounter--;
        }
        else {
            (*it)->Update(GetFrameTime(), walls, softBlocks);
            ++it;
        }
    }

}

void Game::Draw() {
    BeginDrawing();
    ClearBackground(BLACK);

    for (auto& wall : walls) wall.Draw();
    for (auto& softBlock : softBlocks) softBlock.Draw();
    for (auto& bomb : bombs) bomb->Draw();
    for (auto& blast : blasts) {
        blast.Draw();
    }
    for (const auto& enemy : enemies) {
        enemy->Draw(); // Accede al objeto apuntado por unique_ptr
    }

    player.Draw();
    DrawText(TextFormat("Direccion: %d, %d", enemy.GetDirection().x, enemy.GetDirection().y), 100, 90, 40, WHITE);
    DrawText(TextFormat("Vidas: %d", player.GetLife()), 100, 50, 40, WHITE);
    DrawText(TextFormat("Player Pos: %d,%d", player.GetX(), player.GetY()), 600, 50, 40, WHITE);
    DrawText(TextFormat("Blasts: %d", blasts.size()), 1200, 50, 40, WHITE);

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
                }
                else if ((i <= 3 && j <= 3)) {
                    //Lugar SEguro
                }
                else{
                    if (GetRandomValue(1, 4) == 1){
                    softBlocks.emplace_back(i * CELL_SIZE, j * CELL_SIZE + SCREEN_HEIGHT / 5);
                        if (GetRandomValue(1, 4) == 1) {
                            Vector2 v = { i * CELL_SIZE, j * CELL_SIZE + SCREEN_HEIGHT / 5 };
                            AddEnemy(v);
                        }
                    }
                }
            }
            
            else {
                walls.emplace_back(i * CELL_SIZE, j * CELL_SIZE + SCREEN_HEIGHT / 5);
            }
        }
        
    }
}

void Game::AddBomb() {
    
    if (bombs.size() < 10) {
        

        bombs.push_back(new Bomb(((player.GetX() / CELL_SIZE) * CELL_SIZE), ((player.GetY() / CELL_SIZE) * CELL_SIZE) + 15));
        bombs.back()->SetTexture(textureManager.GetTexture(4));
        
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

    // Direcciones: {arriba, abajo, izquierda, derecha}
    Vector2 directions[4] = { {0, -CELL_SIZE}, {0, CELL_SIZE}, {-CELL_SIZE, 0}, {CELL_SIZE, 0} };

    for (auto& dir : directions) {
        for (int i = 1; i <= bomb.GetRange(); i++) {
            Vector2 newPos = { bombPos.x + dir.x * i, bombPos.y + dir.y * i };
            printf("dirX: %f, dirY: %f, posX: %f, posY: %f\n", dir.x, dir.y, newPos.x, newPos.y);
            if (IsBlastBlocked(newPos)) {
                // Si choca con una pared, agregamos un último blast en esa posición y paramos
                //blasts.push_back(Blast(newPos));
                break;
            }

            blasts.push_back(Blast(newPos));
            blasts.back().SetTexture(textureManager.GetTexture(3));
            printf("dirX: %f, dirY: %f, posX: %f, posY: %f, siuuuu!\n", dir.x, dir.y, newPos.x, newPos.y);
        }
    }
}
bool Game::IsBlastBlocked(Vector2 position) {
    for (const auto& wall : walls) {
        if (CheckCollisionRecs({ position.x, position.y + 1, (float) CELL_SIZE, (float)CELL_SIZE}, wall.GetBound())) {
            printf("Ha chocao con: %f, %f\n", wall.GetBound().x, wall.GetBound().y);
            return true;  // Blockeado 
            
        }
    }
    for (auto it = softBlocks.begin(); it != softBlocks.end(); ++it) {
        if (CheckCollisionRecs({ position.x, position.y + 1, (float)CELL_SIZE, (float)CELL_SIZE }, it->GetBound())) {
            printf("Ha chocado con: %f, %f\n", it->GetBound().x, it->GetBound().y);

            
            softBlocks.erase(it);
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

bool Game::CheckBlastDamage(Vector2 pos) {
    Rectangle rec = { pos.x, pos.y, CELL_SIZE, CELL_SIZE};
    for (const auto& blast : blasts) {
        Rectangle r = {blast.position.x, blast.position.y, CELL_SIZE, CELL_SIZE};
        if (CheckCollisionRecs(rec, r)) return true;
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
