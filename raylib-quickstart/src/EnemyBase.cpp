#include "EnemyBase.h"
#include <iostream>
#include "raylib.h"
#include "Constants.h"

Enemy::Enemy(Vector2 startPosition)
    : position(startPosition), speed(2.0f) // Velocidad predeterminada
{}

Enemy::~Enemy() {}

void Enemy::Update(float deltaTime) {
    // Por defecto, no hace nada; las clases derivadas implementan comportamiento específico
    //std::cout << "Enemy base update (sin lógica específica)" << std::endl;

    SetRandomDirection();
    position.x += direction.x * speed;
    position.y += direction.y * speed;
}

void Enemy::Draw() const {
    // Dibuja el enemigo (lógica específica del motor gráfico)
    //std::cout << "Renderizando enemigo en posición: (" << position.x << ", " << position.y << ")" << std::endl;
}

Vector2 Enemy::GetPosition() const {
    return position;
}

void Enemy::SetPosition(Vector2 newPosition) {
    position = newPosition;
}

void Enemy::SetRandomDirection() {
    // Genera una dirección aleatoria para el enemigo
    direction.x = (std::rand() % 3) - 1; // Genera un valor entre -1, 0, 1
    direction.y = (std::rand() % 3) - 1; // Genera un valor entre -1, 0, 1

    // Si la dirección es (0, 0), fuerza otra generación (evita quedarse quieto)
    if (direction.x == 0 && direction.y == 0) {
        SetRandomDirection();
    }
}