#include "Player.h"
#include "Constants.h"
#include "ResourceManager.h"

Player::Player(int startX, int startY) : x(startX), y(startY), life(3) {
    currentFrame = 0;
    framesCounter = 0;
    framesSpeed = 6;
    isDead = false;
    isActive = true;
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
    
    if (idle == false || isDead == true) {
        framesCounter++;
        if (framesCounter >= (60 / framesSpeed)) {
            framesCounter = 0;
            currentFrame++;

            if ((currentFrame > 3 || (prevdirection.x != newdirection.x || prevdirection.y != newdirection.y)) && !isDead) {
                currentFrame = 0;
            }
            else if (isDead && currentFrame > 6) {
                isActive = false;
            }
        }
    }
    else {
        currentFrame = 3;
    }
}
    


void Player::Draw() const {
    Vector2 v = { x , y };
    Rectangle source = { SPRITE_SIZE, 0, SPRITE_SIZE, SPRITE_SIZE };
    Rectangle dest = { x , y  , SPRITE_SIZE * 6.3f , SPRITE_SIZE * 6.3f  };
    Vector2 v2 = { 1, 1 };

    if (!isDead) {
        if (newdirection.x == (float)1 && newdirection.y == (float)0) {
            if (currentFrame != 3) {
                source.x = SPRITE_SIZE * 6 + SPRITE_SIZE * currentFrame;
            }
            else {
                source.x = SPRITE_SIZE * 6 + SPRITE_SIZE * 1;
            }

            source.y = 0;
            DrawTexturePro(texture, source, dest, v2, 0, WHITE);
        }
        else if (newdirection.x == (float)-1 && newdirection.y == (float)0) {
            if (currentFrame != 3) {
                source.x = SPRITE_SIZE * currentFrame;
            }
            else {
                source.x = SPRITE_SIZE * 1;
            }

            source.y = 0;
            DrawTexturePro(texture, source, dest, v2, 0, WHITE);
        }
        else if (newdirection.x == (float)0 && newdirection.y == (float)1) {
            if (currentFrame != 3) {
                source.x = SPRITE_SIZE * 9 + SPRITE_SIZE * currentFrame;
            }
            else {
                source.x = SPRITE_SIZE * 9 + SPRITE_SIZE * 1;
            }

            source.y = 0;
            DrawTexturePro(texture, source, dest, v2, 0, WHITE);
        }
        else if (newdirection.x == (float)0 && newdirection.y == (float)-1) {
            if (currentFrame != 3) {
                source.x = SPRITE_SIZE * 3 + SPRITE_SIZE * currentFrame;

            }
            else {
                source.x = SPRITE_SIZE * 3 + SPRITE_SIZE * 1;
            }
            source.y = 0;
            DrawTexturePro(texture, source, dest, v2, 0, WHITE);
        }
    }
    else {
        source.x = SPRITE_SIZE * 12 + SPRITE_SIZE * currentFrame;
        DrawTexturePro(texture, source, dest, v2, 0, WHITE);
    }
    
}

void Player::DrawHitbox() const {
    DrawRectangleLines(x, y, CELL_SIZE - 10, CELL_SIZE - 10, ORANGE);

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
void Player::SetIdle(bool status){
    idle = status;
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

void Player::Die() {
    isDead = true;
    currentFrame = 0;
    framesCounter = 0;
}

void Player::SetIsDead(bool status)
{
    isDead = status;
}

bool Player::IsActive() {
    return isActive;
}

bool Player::IsDead()
{
    return isDead;
}

void Player::SetActive(bool status) {
    isActive = status;
}
