#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "Vector2D.h"
#include <SDL.h>

class GameObject {
public:
    GameObject(Vector2D position, Vector2D velocity, float rotation = 0.0f, float radius = 10.0f)
        : position(position), velocity(velocity), rotation(rotation), radius(radius), is_active(true) {}

    virtual ~GameObject() = default;

    virtual void update(float deltaTime) = 0;
    virtual void render(SDL_Renderer* renderer) = 0;

    Vector2D getPosition() const { return position; }
    float getRotation() const { return rotation; }
    float getRadius() const { return radius; }
    bool isActive() const { return is_active; }
    void destroy() { is_active = false; }

protected:
    void wrapAroundScreen(int screenWidth, int screenHeight);

    Vector2D position;
    Vector2D velocity;
    float rotation; // In degrees
    float radius;
    bool is_active;
};

#endif // GAMEOBJECT_H