#include "EnemyBase.h"
#include <iostream>
#include "raylib.h"
#include "Constants.h"

Enemy::Enemy(Vector2 startPosition)
    : position(startPosition), speed(2.0f) // Velocidad predeterminada
{}

Enemy::~Enemy() {}

void Enemy::Update(float deltaTime) {
    // Por defecto, no hace nada; las clases derivadas implementan comportamiento espec�fico
    //std::cout << "Enemy base update (sin l�gica espec�fica)" << std::endl;

    SetRandomDirection();
    position.x += direction.x * speed;
    position.y += direction.y * speed;
}

void Enemy::Draw() const {
    // Dibuja el enemigo (l�gica espec�fica del motor gr�fico)
    //std::cout << "Renderizando enemigo en posici�n: (" << position.x << ", " << position.y << ")" << std::endl;
}

Vector2 Enemy::GetPosition() const {
    return position;
}

void Enemy::SetPosition(Vector2 newPosition) {
    position = newPosition;
}

void Enemy::SetRandomDirection() {
    // Genera una direcci�n aleatoria para el enemigo
    direction.x = (std::rand() % 3) - 1; // Genera un valor entre -1, 0, 1
    direction.y = (std::rand() % 3) - 1; // Genera un valor entre -1, 0, 1

    // Si la direcci�n es (0, 0), fuerza otra generaci�n (evita quedarse quieto)
    if (direction.x == 0 && direction.y == 0) {
        SetRandomDirection();
    }
}