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
static int checkCollisions1(Rectangle rec);
static bool OnBomb(void);
static void DrawSoft(void);
static int DestroySoftBlock(Rectangle rec, int bomb);
static void ResetStage(void);


typedef struct Player {
    Rectangle position;
    int vida;
    int invulnerable;
    Color color;
} Player;

typedef struct Blast {
    int radius;
    int timer;
} Blast;

typedef struct Bomb {
    Rectangle position;
    int destroyedBlockUp;
    int destroyedBlockDown;
    int destroyedBlockRight;
    int destroyedBlockLeft;
    bool isActive;
    int timer;
    Blast blast;
} Bomb;



typedef struct WallCell {
    Rectangle box;
}Wall;
typedef struct SoftCell {
    Rectangle box;
}Soft;

void DrawPlayer(Player player) {
    DrawRectangle(player.position.x, player.position.y, player.position.width, player.position.height, player.color);
}
bool onBomb = false;

Wall wallList[120];
int counter = 0;

Soft softList[120];
int softcounter = 0;

Bomb bomblist[10];



// Jugador
Player player = { {MARGIN_X + 50,MARGIN_Y + 50,50 * SCALE,50 * SCALE}, 2, 0, {0,121,241,255} };

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
            int flag = checkCollisions1(player.position);
            if (flag == 1 || flag == 2) {
                player.position.x = tempx;
            }

        }
        if (IsKeyDown(KEY_LEFT)) {
            player.position.x -= 4;
            int flag = checkCollisions1(player.position);
            if (flag == 1 || flag == 2) {
                player.position.x = tempx;
            }

        }
        if (IsKeyDown(KEY_DOWN)) {
            player.position.y += 4;
            int flag = checkCollisions1(player.position);
            if (flag == 1 || flag == 2) {
                player.position.y = tempy;
            }

        }
        if (IsKeyDown(KEY_UP)) {
            player.position.y -= 4;
            int flag = checkCollisions1(player.position);
            if (flag == 1 || flag == 2) {
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
                    bomblist[i].destroyedBlockRight = 10;
                    bomblist[i].destroyedBlockLeft = 10;
                    bomblist[i].destroyedBlockUp = 10;
                    bomblist[i].destroyedBlockDown = 10;
                    bombcounter--;
                }
            }
        }





        // Dibujo
        BeginDrawing();
        ClearBackground(BLACK);
        DrawSoft();
        // Dibujar jugador y bomba
        DrawPlayer(player);

        DrawBombs();

        DrawGrid();
        char str[200];
        sprintf(str, "%d", player.vida);
        DrawText(str, 100, 50, 40, WHITE);
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
void DrawSoft() {
    for (int i = 0; i < softcounter; i++) {
        DrawRectangle(softList[i].box.x, softList[i].box.y, softList[i].box.width, softList[i].box.height, ORANGE);
    }
}
void DrawBombs() {
    for (int i = 0; i < MAX_BOMBS; i++) {
        if (bomblist[i].timer > 0) {
            DrawCircle(bomblist[i].position.x, bomblist[i].position.y, CELL_SIZE / 2, DARKGRAY);
        }
        else if (bomblist[i].blast.timer > 0) {
            Rectangle h = { bomblist[i].position.x + 25,bomblist[i].position.y - 25 ,50,50 };
            int initialX = h.x;
            int initialY = h.y;


            //DrawRectangle(h.x, h.y, h.width, h.height, ORANGE);//Derecha
            for (int j = 1; j < BOMB_RADIUS + 1; j++) {
                int flag = checkCollisions1(h);

                if (flag == 0 && bomblist[i].destroyedBlockRight > j) {
                    DrawRectangle(h.x, h.y, 50, 50, RED);//Derecha
                    if (CheckCollisionRecs(h, player.position) == true) {
                        //Stage Reset
                        printf("Derecha");
                        ResetStage();

                    }
                }
                else if (flag == 1) {
                    h.x = initialX;
                    break;
                }
                else if (flag == 2 && bomblist[i].destroyedBlockRight == 10) {

                    DestroySoftBlock(h, i);
                    bomblist[i].destroyedBlockRight = j;

                    break;
                }

                h.x = h.x + 50;
            }
            h.x = initialX - 100;



            for (int j = 0; j < BOMB_RADIUS; j++) {
                int flag = checkCollisions1(h);

                if (flag == 0 && bomblist[i].destroyedBlockLeft > j) {
                    DrawRectangle(h.x, h.y, 50, 50, BLUE);//Izquierda
                    if (CheckCollisionRecs(h, player.position) == true) {
                        //Stage Reset
                        printf("Izquierda");
                        ResetStage();

                    }
                }
                else if (flag == 1) {
                    h.x = initialX;
                    break;
                }
                else if (flag == 2 && bomblist[i].destroyedBlockLeft == 10) {

                    DestroySoftBlock(h, i);
                    bomblist[i].destroyedBlockLeft = j;

                    break;

                }

                h.x = h.x - 50;

            }
            h.x = initialX - 50;



            h.y = h.y + 50;
            //printf("HAHAHAH");
            for (int j = 0; j < BOMB_RADIUS; j++) {
                int flag = checkCollisions1(h);
                if (flag == 0 && bomblist[i].destroyedBlockDown > j) {
                    DrawRectangle(h.x, h.y, 50, 50, GREEN);//Abajo
                    if (CheckCollisionRecs(h, player.position) == true) {
                        //Stage Reset
                        printf("Abajo");
                        ResetStage();

                    }
                }
                else if (flag == 1) {
                    h.y = initialY;
                    break;
                }
                else if (flag == 2 && bomblist[i].destroyedBlockDown == 10) {

                    DestroySoftBlock(h, i);
                    bomblist[i].destroyedBlockDown = j;

                    break;

                }
                h.y = h.y + 50;
            }

            h.y = initialY;
            h.y = h.y - 50;
            for (int j = 0; j < BOMB_RADIUS; j++) {
                int flag = checkCollisions1(h);
                if (flag == 0 && bomblist[i].destroyedBlockUp > j) {
                    DrawRectangle(h.x, h.y, 50, 50, YELLOW);//Arriba
                    if (CheckCollisionRecs(h, player.position) == true) {
                        //Stage Reset
                        printf("Arriba");
                        ResetStage();

                    }

                }
                else if (flag == 1) {

                    h.y = initialY;
                    break;
                }
                else if (flag == 2 && bomblist[i].destroyedBlockUp == 10) {

                    DestroySoftBlock(h, i);
                    bomblist[i].destroyedBlockUp = j;

                    break;

                }
                h.y = h.y - 50;
            }
            h.y = initialY;

        }

    }
}
void AddWalls() {
    counter = 0;
    softcounter = 0;
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
                else if ((i <= 3 && j <= 3)) {
                }
                else if (GetRandomValue(1, 4) == 1) {
                    softList[softcounter].box.x = (i * (50 * SCALE) + MARGIN_X);
                    softList[softcounter].box.y = (j * (50 * SCALE) + MARGIN_Y);
                    softList[softcounter].box.width = 50 * SCALE;
                    softList[softcounter].box.height = 50 * SCALE;
                    softcounter++;
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
                bomblist[i].destroyedBlockUp = 10;
                bomblist[i].destroyedBlockDown = 10;
                bomblist[i].destroyedBlockLeft = 10;
                bomblist[i].destroyedBlockRight = 10;
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
int checkCollisions1(Rectangle rec) {
    for (int i = 0; i < counter - 1; i++) {
        if (CheckCollisionRecs(rec, wallList[i].box) == true)
        {
            return 1;
        }
    }
    for (int i = 0; i < softcounter - 1; i++) {
        if (CheckCollisionRecs(rec, softList[i].box) == true)
        {
            return 2;
        }
    }
    for (int i = 0; i < MAX_BOMBS; i++) {
        Vector2 v = { bomblist[i].position.x, bomblist[i].position.y };
        if (CheckCollisionCircleRec(v, CELL_SIZE / 2, player.position) && onBomb == false) {
            return 3;
        }
    }

    return 0;
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
int DestroySoftBlock(Rectangle rec, int bomb) {
    for (int i = 0; i < softcounter - 1; i++) {
        if (CheckCollisionRecs(rec, softList[i].box) == true)
        {

            softList[i].box.x = -3;
            softList[i].box.y = -3;
            softList[i].box.width = 0;
            softList[i].box.height = 0;

            return 1;
        }
    }
    return 0;
}
void ResetStage() {
    player.position.x = MARGIN_X + 50;
    player.position.y = MARGIN_Y + 50;
    player.vida = player.vida - 1;
    counter = 0;
    softcounter = 0;

    for (int i = 0; i < MAX_BOMBS; i++) {
        bomblist[i].position.x = 0;
        bomblist[i].position.y = 0;
        bomblist[i].isActive = false;
        bomblist[i].destroyedBlockUp = 10;
        bomblist[i].destroyedBlockDown = 10;
        bomblist[i].destroyedBlockLeft = 10;
        bomblist[i].destroyedBlockRight = 10;
        bomblist[i].timer = BOMB_TIMER;
        Blast b = { BOMB_RADIUS,BOMB_TIMER };
        bomblist[i].blast = b;
    }

    AddWalls();

}