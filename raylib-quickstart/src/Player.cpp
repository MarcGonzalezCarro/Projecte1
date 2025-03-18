#include "Player.h"
#include "Constants.h"
#include "TextureManager.h"

Player::Player(int startX, int startY) : x(startX), y(startY), life(3) {
    currentFrame = 0;
    framesCounter = 0;
    framesSpeed = 6;
}

Player::~Player() {
    UnloadTexture(texture);  // Liberar memoria de la textura
}

void Player::Move(int dx, int dy, Vector2 dir) {
    x += dx;
    y += dy;
    prevdirection = newdirection;
    newdirection = dir;
}

void Player::Update() {
    framesCounter++;
    if (framesCounter >= (60 / framesSpeed)) {
        framesCounter = 0;
        currentFrame++;

        if (currentFrame > 2 || (prevdirection.x != newdirection.x || prevdirection.y != newdirection.y)) { 
            currentFrame = 0; 
        }
    }
}

void Player::Draw() const {
    Vector2 v = { x + CAMERA_OFFSET_X, y + CAMERA_OFFSET_Y};
    Rectangle source = { SPRITE_SIZE, 0, SPRITE_SIZE, SPRITE_SIZE };
    Rectangle dest = { x + CAMERA_OFFSET_X, y + CAMERA_OFFSET_Y, SPRITE_SIZE * 6, SPRITE_SIZE * 6 };
    Vector2 v2 = { 1, 1 };

    DrawText(TextFormat("prevDireccion: %f, %f", prevdirection.x, prevdirection.y), 200, 120, 40, WHITE);
    DrawText(TextFormat("newDireccion: %f, %f", newdirection.x, newdirection.y), 200, 150, 40, WHITE);
    DrawText(TextFormat("currentFrame: %d, %d", currentFrame, framesCounter), 200, 180, 40, WHITE);
    if (newdirection.x == (float) 1 && newdirection.y == (float) 0) {
        source.x = SPRITE_SIZE * 6 + SPRITE_SIZE * currentFrame;
        source.y = 0;
        DrawTexturePro(texture, source, dest, v2, 0, WHITE);
    }
    else if (newdirection.x == (float) -1 && newdirection.y == (float) 0) {
        source.x = SPRITE_SIZE * currentFrame;
        source.y = 0;
        DrawTexturePro(texture, source, dest, v2, 0, WHITE);
    }else if (newdirection.x == (float) 0 && newdirection.y == (float) 1) {
        source.x = SPRITE_SIZE * 9 + SPRITE_SIZE * currentFrame;
        source.y = 0;
        DrawTexturePro(texture, source, dest, v2, 0, WHITE);
    }else if (newdirection.x == (float) 0 && newdirection.y == (float) -1) {
        source.x = SPRITE_SIZE * 3 + SPRITE_SIZE * currentFrame;
        source.y = 0;
        DrawTexturePro(texture, source, dest, v2, 0, WHITE);
    }
    
}

void Player::SetPosition(int newX, int newY) {
    x = newX;
    y = newY;
}

void Player::SetDirection(Vector2 dir) {
    newdirection = dir;
}

void Player::SetX(int newX) {
    x = newX;
}

void Player::SetY(int newY) {
    y = newY;
}

void Player::SetTexture(Texture2D tex) {
    texture = tex;
}

Rectangle Player::GetBounds() const {
    return { (float)x, (float)y, (float)CELL_SIZE - 10 , (float)CELL_SIZE - 10};
}

int Player::GetX() const {
    return x;
}

int Player::GetY() const {
    return y;
}

int Player::GetLife() const {
    return life;
}

void Player::DecreaseLife() {
    if (life > 0) life--;
}
