#ifndef BULLET_H
#define BULLET_H

#include "GameObject.h"

class Bullet : public GameObject {
public:
    Bullet(Vector2D position, Vector2D velocity);

    void update(float deltaTime) override;
    void render(SDL_Renderer* renderer) override;

    float getLifetime() const { return lifetime; }

private:
    float lifetime; // in seconds
};

#endif // BULLET_H