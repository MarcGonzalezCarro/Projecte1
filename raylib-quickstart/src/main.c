#include "raylib.h"

#define GRID_ROWS 30
#define GRID_COLUMNS 12
#define CELL_SIZE 40

#define MIN_X 20
#define MAX_X 1600
#define MIN_Y 20
#define MAX_Y 800

static void DrawGrid(void);
static void AddWalls(void);
static bool checkCollisions1(void);

typedef struct Player {
    Rectangle position;
    Color color;
} Player;

typedef struct Bomb {
    Rectangle position;
    bool isActive;
    int timer;
} Bomb;

typedef struct WallCell {
    Rectangle box;
}Wall;

void DrawPlayer(Player player) {
    DrawRectangle(player.position.x, player.position.y, player.position.width, player.position.height, player.color);
}

void DrawBomb(Bomb bomb) {
    if (bomb.isActive) {
        DrawCircle(bomb.position.x, bomb.position.y, CELL_SIZE / 4, DARKGRAY);
    }
}
Wall wallList[100];
// Jugador
Player player = { {400,400,30,40}, {0,121,241,255} };
int counter = 0;
int main(void) {
    // Inicialización de la ventana
    InitWindow(1920, 1080, "test");
    SetTargetFPS(60);




    // Bombas
    Bomb bomb = { {0, 0}, false, 0 };

    AddWalls();
    while (!WindowShouldClose()) {

        int tempx = player.position.x;
        int tempy = player.position.y;
        // Manejo de entrada

        if (IsKeyDown(KEY_RIGHT)) {

            player.position.x += 4;
            if (checkCollisions1() == true || player.position.x >= MAX_X) {
                player.position.x = tempx;
            }
        }
        if (IsKeyDown(KEY_LEFT)) {
            player.position.x -= 4;
            if (checkCollisions1() == true || player.position.x <= MIN_X) {
                player.position.x = tempx;
            }
        }
        if (IsKeyDown(KEY_DOWN)) {
            player.position.y += 4;
            if (checkCollisions1() == true || player.position.y >= MAX_Y) {
                player.position.y = tempy;
            }
        }
        if (IsKeyDown(KEY_UP)) {
            player.position.y -= 4;
            if (checkCollisions1() == true || player.position.y <= MIN_Y) {
                player.position.y = tempy;
            }
        }


        
        // Colocar bomba
        if (IsKeyPressed(KEY_SPACE) && !bomb.isActive) {
            bomb.position = player.position;
            bomb.isActive = true;
            bomb.timer = 60;  // Temporizador para la bomba (1 segundo)
        }

        // Actualizar bomba
        if (bomb.isActive) {
            bomb.timer--;
            if (bomb.timer <= 0) {
                bomb.isActive = false;
            }
        }



        // Dibujo
        BeginDrawing();
        ClearBackground(BLACK);
        //DrawRectangle(player.position.x, player.position.y, player.position.width, player.position.height, GREEN);
        // Dibujar jugador y bomba
        DrawPlayer(player);

        DrawBomb(bomb);

        DrawGrid();


        EndDrawing();
    }

    // Cerrar ventana
    CloseWindow();

    return 0;
}
void DrawGrid() {
    for (int i = 0; i < 100; i++) {
        DrawRectangle(wallList[i].box.x, wallList[i].box.y, wallList[i].box.width, wallList[i].box.height, WHITE);
    }

}
void AddWalls() {
    counter = 0;
    for (int i = 0; i < GRID_ROWS; i++) {
        for (int j = 0; j < GRID_COLUMNS; j++) {
            if (i != 0 && j != 0 && i != GRID_ROWS - 1 && j != GRID_COLUMNS - 1) {
                if (i % 2 == 0 && j % 2 == 0) {

                    wallList[counter].box.x = i * 50 + 150;
                    wallList[counter].box.y = j * 50 + 150;
                    wallList[counter].box.width = 50;
                    wallList[counter].box.height = 50;
                    counter++;
                }
            }
        }
    }
}
bool checkCollisions1() {
    for (int i = 0; i < counter - 1; i++) {
        if (CheckCollisionRecs(player.position, wallList[i].box) == true)
        {
            return true;
        }
    }
    return false;
}
