#ifndef PLAYER_H
#define PLAYER_H

#include "GameObject.h"

class Player : public GameObject {
public:
    Player(Vector2D position);

    void handleInput(const Uint8* keyStates);
    void update(float deltaTime) override;
    void render(SDL_Renderer* renderer) override;

    void reset();
    void loseLife() { lives--; }
    int getLives() const { return lives; }
    int getScore() const { return score; }
    void addScore(int points) { score += points; }

    bool isShooting() const { return shooting; }
    void setShooting(bool is_shooting) { shooting = is_shooting; }

private:
    static constexpr float THRUST_FORCE = 250.0f;
    static constexpr float ROTATION_SPEED = 200.0f; // degrees per second
    static constexpr float FRICTION = 0.995f;

    int lives;
    int score;
    bool shooting;
};

#endif // PLAYER_H