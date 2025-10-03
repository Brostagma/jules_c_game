#include "Player.h"
#include <cmath>
#include <SDL.h>

#define PI 3.14159265

Player::Player(Vector2D position)
    : GameObject(position, {0, 0}, 0.0f, 10.0f), lives(3), score(0), shooting(false) {}

void Player::reset() {
    position = {400, 300}; // Center of the screen
    velocity = {0, 0};
    rotation = 0.0f;
    is_active = true;
}

void Player::handleInput(const Uint8* keyStates) {
    if (keyStates[SDL_SCANCODE_LEFT]) {
        rotation -= ROTATION_SPEED * (1.0f / 60.0f); // Assuming 60 FPS for now
    }
    if (keyStates[SDL_SCANCODE_RIGHT]) {
        rotation += ROTATION_SPEED * (1.0f / 60.0f);
    }
    if (keyStates[SDL_SCANCODE_UP]) {
        float rad = rotation * PI / 180.0f;
        velocity.x += std::cos(rad) * THRUST_FORCE * (1.0f / 60.0f);
        velocity.y += std::sin(rad) * THRUST_FORCE * (1.0f / 60.0f);
    }
    if (keyStates[SDL_SCANCODE_SPACE]) {
        shooting = true;
    } else {
        shooting = false;
    }
}

void Player::update(float deltaTime) {
    // Apply friction
    velocity = velocity * FRICTION;

    // Update position
    position += velocity * deltaTime;

    // Wrap around screen
    wrapAroundScreen(800, 600); // Assuming 800x600 screen
}

void Player::render(SDL_Renderer* renderer) {
    if (!is_active) return;

    // Set draw color to white
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    // Convert rotation to radians for trig functions
    float rad = rotation * PI / 180.0f;

    // Define the 3 vertices of the ship triangle relative to its center
    Vector2D p1 = { position.x + cos(rad) * 15.0f, position.y + sin(rad) * 15.0f };
    Vector2D p2 = { position.x + cos(rad + 2.5) * 15.0f, position.y + sin(rad + 2.5) * 15.0f };
    Vector2D p3 = { position.x + cos(rad - 2.5) * 15.0f, position.y + sin(rad - 2.5) * 15.0f };

    // Draw the triangle
    SDL_RenderDrawLine(renderer, (int)p1.x, (int)p1.y, (int)p2.x, (int)p2.y);
    SDL_RenderDrawLine(renderer, (int)p2.x, (int)p2.y, (int)p3.x, (int)p3.y);
    SDL_RenderDrawLine(renderer, (int)p3.x, (int)p3.y, (int)p1.x, (int)p1.y);
}