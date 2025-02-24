#include "raylib.h"
#include "stdio.h"

#define MARGIN_X 60
#define MARGIN_Y 300
#define GRID_ROWS 31
#define GRID_COLUMNS 13
#define CELL_SIZE 40

#define MAX_BOMBS 10
#define BOMB_TIMER 150
#define BOMB_RADIUS 4

#define SCALE 1


static void DrawGrid(void);
static void DrawBombs(void);
static void AddWalls(void);
static void AddBomb(void);
static void ExplodeBombs(void);
static bool checkCollisions1(void);
static bool OnBomb(void);

typedef struct Player {
    Rectangle position;
    Color color;
} Player;

typedef struct Blast {
    int radius;
    int timer;
} Blast;

typedef struct Bomb {
    Rectangle position;
    bool isActive;
    int timer;
    Blast blast;
} Bomb;



typedef struct WallCell {
    Rectangle box;
}Wall;

void DrawPlayer(Player player) {
    DrawRectangle(player.position.x, player.position.y, player.position.width, player.position.height, player.color);
}
bool onBomb = false;

Wall wallList[120];
Bomb bomblist[10];
int bombGrid[10][30];
// Jugador
Player player = { {MARGIN_X + 55,MARGIN_Y + 55,30 * SCALE,40 * SCALE}, {0,121,241,255} };
int counter = 0;
int bombcounter = 0;
int main(void) {
    // Inicialización de la ventana
    InitWindow(1920, 1080, "test");
    SetTargetFPS(60);

    AddWalls();
    bomblist[0].isActive = false;

    while (!WindowShouldClose()) {

        int tempx = player.position.x;
        int tempy = player.position.y;
        onBomb = OnBomb();
        // Manejo de entrada

        if (IsKeyDown(KEY_RIGHT)) {

            player.position.x += 4;
            if (checkCollisions1() == true) {
                player.position.x = tempx;
            }
        }
        if (IsKeyDown(KEY_LEFT)) {
            player.position.x -= 4;
            if (checkCollisions1() == true) {
                player.position.x = tempx;
            }
        }
        if (IsKeyDown(KEY_DOWN)) {
            player.position.y += 4;
            if (checkCollisions1() == true) {
                player.position.y = tempy;
            }
        }
        if (IsKeyDown(KEY_UP)) {
            player.position.y -= 4;
            if (checkCollisions1() == true) {
                player.position.y = tempy;
            }
        }



        // Colocar bomba
        if (IsKeyPressed(KEY_SPACE)) {
            AddBomb();
            //bomb.position = player.position;
            //bomb.isActive = true;
        }
        if (IsKeyPressed(KEY_E)) {
            ExplodeBombs();
        }

        for (int i = 0; i < MAX_BOMBS; i++) {
            if (bomblist[i].isActive) {
                bomblist[i].timer--;
                if (bomblist[i].timer <= 0) {


                }
            }
            if (bomblist[i].blast.timer > 0 && bomblist[i].timer <= 0) {
                bomblist[i].blast.timer--;
                if (bomblist[i].blast.timer <= 0) {
                    bomblist[i].position.x = -4;
                    bomblist[i].position.y = -4;
                    bomblist[i].isActive = false;
                    bombcounter--;
                }
            }
        }





        // Dibujo
        BeginDrawing();
        ClearBackground(BLACK);

        // Dibujar jugador y bomba
        DrawPlayer(player);

        DrawBombs();

        DrawGrid();


        EndDrawing();
    }

    // Cerrar ventana
    CloseWindow();

    return 0;
}
void DrawGrid() {
    for (int i = 0; i < counter; i++) {
        DrawRectangle(wallList[i].box.x, wallList[i].box.y, wallList[i].box.width, wallList[i].box.height, WHITE);
    }

}
void DrawBombs() {
    for (int i = 0; i < MAX_BOMBS; i++) {
        if (bomblist[i].timer > 0) {
            DrawCircle(bomblist[i].position.x, bomblist[i].position.y, CELL_SIZE / 2, DARKGRAY);
        }
        else if (bomblist[i].blast.timer > 0) {
            for (int j = 0; j < BOMB_RADIUS; j++) {
                DrawRectangle(bomblist[i].position.x + 25 + (50 * j), bomblist[i].position.y - 25, 50, 50, RED);//Derecha
                DrawRectangle(bomblist[i].position.x - 75 - (50 * j), bomblist[i].position.y - 25, 50, 50, BLUE);//Izquierda
                DrawRectangle(bomblist[i].position.x - 25, bomblist[i].position.y + 25 + (50 * j), 50, 50, GREEN);//Abajo
                DrawRectangle(bomblist[i].position.x - 25, bomblist[i].position.y - 75 - (50 * j), 50, 50, YELLOW);//Arriba
            }
        }

    }
}
void AddWalls() {
    counter = 0;
    for (int i = 0; i < GRID_ROWS; i++) {
        for (int j = 0; j < GRID_COLUMNS; j++) {
            if (i != 0 && j != 0 && i != GRID_ROWS - 1 && j != GRID_COLUMNS - 1) {
                if (i % 2 == 0 && j % 2 == 0) {

                    wallList[counter].box.x = i * (50 * SCALE) + MARGIN_X;
                    wallList[counter].box.y = j * (50 * SCALE) + MARGIN_Y;
                    wallList[counter].box.width = 50 * SCALE;
                    wallList[counter].box.height = 50 * SCALE;
                    counter++;
                }
            }
            else {
                wallList[counter].box.x = i * (50 * SCALE) + MARGIN_X;
                wallList[counter].box.y = j * (50 * SCALE) + MARGIN_Y;
                wallList[counter].box.width = 50 * SCALE;
                wallList[counter].box.height = 50 * SCALE;
                counter++;
            }
        }
    }
}
void AddBomb() {
    if (bombcounter < MAX_BOMBS) {
        for (int i = 0; i < MAX_BOMBS; i++) {
            if (bomblist[i].isActive == false) {

                bomblist[i].position.x = (((int)player.position.x / 50) * 50) + 35;
                bomblist[i].position.y = ((((int)player.position.y + 15) / 50) * 50) + 25;
                bomblist[i].isActive = true;
                bomblist[i].timer = BOMB_TIMER;
                Blast b = { BOMB_RADIUS,BOMB_TIMER };
                bomblist[i].blast = b;
                printf("La operacion: %f entre %d es: %d", player.position.x, 50, ((int)player.position.x / 50) * 50);
                printf("Pos x: %f\nPos y: %f\n", player.position.x, player.position.y);
                return;
            }
        }

        bombcounter++;
    }
    else {
        //Añadir sonido o algo
    }

}
void ExplodeBombs() {

    bombcounter = 0;
}
bool checkCollisions1() {
    for (int i = 0; i < counter - 1; i++) {
        if (CheckCollisionRecs(player.position, wallList[i].box) == true)
        {
            return true;
        }
    }
    for (int i = 0; i < MAX_BOMBS; i++) {
        Vector2 v = { bomblist[i].position.x, bomblist[i].position.y };
        if (CheckCollisionCircleRec(v, CELL_SIZE / 2, player.position) && onBomb == false) {
            return true;
        }
    }

    return false;
}
bool OnBomb() {
    for (int i = 0; i < MAX_BOMBS; i++) {
        Vector2 v = { bomblist[i].position.x, bomblist[i].position.y };
        if (CheckCollisionCircleRec(v, CELL_SIZE / 2, player.position)) {
            return true;
        }
    }
    return false;
}
