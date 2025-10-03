#include "Asteroid.h"
#include <random>

Asteroid::Asteroid(Vector2D position, Vector2D velocity, AsteroidSize size)
    : GameObject(position, velocity), size(size) {
    switch (size) {
        case AsteroidSize::LARGE:
            radius = 40.0f;
            break;
        case AsteroidSize::MEDIUM:
            radius = 20.0f;
            break;
        case AsteroidSize::SMALL:
            radius = 10.0f;
            break;
    }
    generateModel();
}

void Asteroid::generateModel() {
    // Create a random-looking polygon
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(12, 20);
    int num_vertices = distrib(gen);

    for (int i = 0; i < num_vertices; ++i) {
        float angle = (float)i / num_vertices * 2.0f * 3.14159265f;
        std::uniform_real_distribution<> r_dist(radius * 0.7, radius * 1.3);
        float r = r_dist(gen);
        model.push_back({r * cos(angle), r * sin(angle)});
    }
}


void Asteroid::update(float deltaTime) {
    position += velocity * deltaTime;
    wrapAroundScreen(800, 600); // Assuming 800x600
}

void Asteroid::render(SDL_Renderer* renderer) {
    if (!is_active) return;

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    for (size_t i = 0; i < model.size(); ++i) {
        Vector2D p1 = position + model[i];
        Vector2D p2 = position + model[(i + 1) % model.size()];
        SDL_RenderDrawLine(renderer, (int)p1.x, (int)p1.y, (int)p2.x, (int)p2.y);
    }
}