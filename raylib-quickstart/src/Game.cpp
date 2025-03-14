#include "Game.h"
#include "Constants.h"
#include <iostream>
#include "Blast.h"
#include "Ballom.h"

Game::Game() : player(INITIAL_PLAYER_X, INITIAL_PLAYER_Y), onBomb(false) {
    
    AddWalls();
}

std::vector<Blast> blasts; // Definición de la variable global
Vector2 enemyStartPos = { INITIAL_PLAYER_X, INITIAL_PLAYER_Y};
Ballom enemy(enemyStartPos);

void Game::Run() {
    
    
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
        player.Move(4, 0);
        if (CheckCollisions(player.GetBounds()) != 0) {
            player.SetX(prevX);
        }else if (player.GetX() > CAMERA_BORDER_MIN_X && player.GetX() < CAMERA_BORDER_MAX_X) {
            //MoverDerecha
            CAMERA_OFFSET_X -= 4;
            
        }
        
    }
    if (IsKeyDown(KEY_LEFT)) { 
        player.Move(-4, 0); 
        if (CheckCollisions(player.GetBounds()) != 0) {
            player.SetX(prevX);
        }else if (player.GetX() > CAMERA_BORDER_MIN_X && player.GetX() < CAMERA_BORDER_MAX_X) {
            //MoverIzquierda
            
            CAMERA_OFFSET_X += 4;
        }
        
    }
    if (IsKeyDown(KEY_DOWN)) { 
        player.Move(0, 4); 
        if (CheckCollisions(player.GetBounds()) != 0) {
            player.SetY(prevY);
        }
        else if (player.GetY() > CAMERA_BORDER_MIN_Y && player.GetY() < CAMERA_BORDER_MAX_Y) {
            //MoverIzquierda
            CAMERA_OFFSET_Y -= 4;
        }
    }
    if (IsKeyDown(KEY_UP)) { 
        player.Move(0, -4); 
        if (CheckCollisions(player.GetBounds()) != 0) {
            player.SetY(prevY);
        }
        else if (player.GetY() > CAMERA_BORDER_MIN_Y && player.GetY() < CAMERA_BORDER_MAX_Y) {
            //MoverIzquierda
            CAMERA_OFFSET_Y += 4;
        }
    }

    

    if (IsKeyPressed(KEY_SPACE)) AddBomb();
    if (IsKeyPressed(KEY_E)) ExplodeBombs();

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
    for (int i = 0; i < 100; ++i) { // Simulación de 100 frames
        enemy.Update(GetFrameTime(), walls);
        enemy.Draw();
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
    player.Draw();
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
                else if (GetRandomValue(1, 4) == 1) {
                    softBlocks.emplace_back(i * CELL_SIZE, j * CELL_SIZE + SCREEN_HEIGHT / 5);
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
int Game::CheckCollisions(Rectangle rec) {
    for (const auto& wall : walls) {
        if (CheckCollisionRecs(rec, wall.GetBound())) return 1;
    }
    for (const auto& softBlock : softBlocks) {
        if (CheckCollisionRecs(rec, softBlock.GetBound())) return 2;
    }
    for (const auto& bomb : bombs) {
        if (CheckCollisionRecs(rec, bomb->GetBounds()) && !onBomb) return 3;
    }
    return 0;
}



void Game::ResetStage() {
    player.SetPosition(100, 100);
    player.DecreaseLife();
    bombs.clear();
    AddWalls();
}
