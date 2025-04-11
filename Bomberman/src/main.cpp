#include "Game.h"
#include "raylib.h"
#include "Constants.h"

int main() {

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Bomberman");
    SetTargetFPS(60);

    Game game;
    game.Run(); // Ejecuta el juego
    return 0;
}