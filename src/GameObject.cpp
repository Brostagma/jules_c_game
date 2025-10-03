#include "GameObject.h"

void GameObject::update(float deltaTime) {
    position += velocity * deltaTime;
}

void GameObject::wrapAroundScreen(int screenWidth, int screenHeight) {
    if (position.x < 0) position.x = screenWidth;
    if (position.x > screenWidth) position.x = 0;
    if (position.y < 0) position.y = screenHeight;
    if (position.y > screenHeight) position.y = 0;
}