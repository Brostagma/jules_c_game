#ifndef GAME_H
#define GAME_H

#include <SDL.h>
#include <vector>
#include <memory>
#include "GameObject.h"
#include "Player.h"
#include "Asteroid.h"
#include "Bullet.h"
#include "TextRenderer.h"

enum class GameState {
    PLAYING,
    GAME_OVER
};

class Game {
public:
    Game();
    ~Game();

    bool init();
    void run();

private:
    void gameLoop();
    void processInput();
    void update(float deltaTime);
    void render();
    void handleCollisions();
    void spawnAsteroids(int count);
    void resetGame();

    SDL_Window* window;
    SDL_Renderer* renderer;
    std::unique_ptr<TextRenderer> textRenderer;

    bool isRunning;
    GameState gameState;

    std::unique_ptr<Player> player;
    std::vector<std::unique_ptr<Asteroid>> asteroids;
    std::vector<std::unique_ptr<Bullet>> bullets;

    const int SCREEN_WIDTH = 800;
    const int SCREEN_HEIGHT = 600;

    float shootCooldown;
    const float SHOOT_COOLDOWN_TIME = 0.25f; // seconds
};

#endif // GAME_H