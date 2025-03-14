#include "Player.h"
#include "Constants.h"

Player::Player(int startX, int startY) : x(startX), y(startY), life(3) {
    texture = LoadTexture("resources/player01.png");
}

Player::~Player() {
    UnloadTexture(texture);  // Liberar memoria de la textura
}

void Player::Move(int dx, int dy) {
    x += dx;
    y += dy;
}

void Player::Draw() const {
    Vector2 v = { x + CAMERA_OFFSET_X, y + CAMERA_OFFSET_Y};
    DrawTextureEx(texture, v, 0, 6, WHITE);
}

void Player::SetPosition(int newX, int newY) {
    x = newX;
    y = newY;
}

void Player::SetX(int newX) {
    x = newX;
}

void Player::SetY(int newY) {
    y = newY;
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
