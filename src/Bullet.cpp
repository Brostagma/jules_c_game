#include "Bullet.h"

Bullet::Bullet(Vector2D position, Vector2D velocity)
    : GameObject(position, velocity, 0.0f, 2.0f), lifetime(1.5f) {}

void Bullet::update(float deltaTime) {
    position += velocity * deltaTime;
    lifetime -= deltaTime;
    if (lifetime <= 0) {
        is_active = false;
    }
    wrapAroundScreen(800, 600);
}

void Bullet::render(SDL_Renderer* renderer) {
    if (!is_active) return;
    SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255); // Yellow
    SDL_Rect rect = { (int)position.x - 2, (int)position.y - 2, 4, 4 };
    SDL_RenderFillRect(renderer, &rect);
}