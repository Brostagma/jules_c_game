#ifndef ASTEROID_H
#define ASTEROID_H

#include "GameObject.h"
#include <vector>

enum class AsteroidSize {
    LARGE,
    MEDIUM,
    SMALL
};

class Asteroid : public GameObject {
public:
    Asteroid(Vector2D position, Vector2D velocity, AsteroidSize size);

    void update(float deltaTime) override;
    void render(SDL_Renderer* renderer) override;

    AsteroidSize getSize() const { return size; }

private:
    AsteroidSize size;
    std::vector<Vector2D> model; // Points for the polygon shape
    void generateModel();
};

#endif // ASTEROID_H