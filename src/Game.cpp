#include "Game.h"
#include <iostream>
#include <string>
#include <random>
#include <algorithm>

#define PI 3.14159265

Game::Game()
    : window(nullptr), renderer(nullptr), isRunning(false), gameState(GameState::PLAYING), shootCooldown(0.0f) {}

Game::~Game() {
    if (renderer) SDL_DestroyRenderer(renderer);
    if (window) SDL_DestroyWindow(window);
    SDL_Quit();
}

bool Game::init() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }

    window = SDL_CreateWindow("Cosmic Shards", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (!window) {
        std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer) {
        std::cerr << "Renderer could not be created! SDL Error: " << SDL_GetError() << std::endl;
        return false;
    }

    // This font is a default on Windows.
    // In a real distributable game, you'd bundle a .ttf file.
    textRenderer = std::make_unique<TextRenderer>(renderer, "C:/Windows/Fonts/arial.ttf", 24);


    resetGame();

    isRunning = true;
    return true;
}

void Game::resetGame() {
    player = std::make_unique<Player>(Vector2D(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f));
    asteroids.clear();
    bullets.clear();
    spawnAsteroids(4);
    gameState = GameState::PLAYING;
}


void Game::spawnAsteroids(int count) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> pos_x_dist(0, SCREEN_WIDTH);
    std::uniform_real_distribution<> pos_y_dist(0, SCREEN_HEIGHT);
    std::uniform_real_distribution<> vel_dist(-50.0f, 50.0f);

    for (int i = 0; i < count; ++i) {
        Vector2D pos;
        // Ensure asteroids don't spawn on top of the player
        do {
            pos = { (float)pos_x_dist(gen), (float)pos_y_dist(gen) };
        } while ((pos - player->getPosition()).magnitude() < 100.0f);

        Vector2D vel = { (float)vel_dist(gen), (float)vel_dist(gen) };
        asteroids.push_back(std::make_unique<Asteroid>(pos, vel, AsteroidSize::LARGE));
    }
}


void Game::run() {
    Uint32 lastTime = SDL_GetTicks();

    while (isRunning) {
        Uint32 currentTime = SDL_GetTicks();
        float deltaTime = (currentTime - lastTime) / 1000.0f;
        lastTime = currentTime;

        processInput();
        update(deltaTime);
        render();
    }
}

void Game::processInput() {
    SDL_Event e;
    while (SDL_PollEvent(&e) != 0) {
        if (e.type == SDL_QUIT) {
            isRunning = false;
        }
        if (gameState == GameState::GAME_OVER && e.type == SDL_KEYDOWN) {
             if (e.key.keysym.sym == SDLK_r) {
                resetGame();
            }
        }
    }

    const Uint8* keyStates = SDL_GetKeyboardState(NULL);
    if (gameState == GameState::PLAYING) {
        player->handleInput(keyStates);
    }
}

void Game::update(float deltaTime) {
    if (gameState != GameState::PLAYING) return;

    // Cooldown management
    if (shootCooldown > 0) {
        shootCooldown -= deltaTime;
    }

    // Player shooting
    if (player->isShooting() && shootCooldown <= 0) {
        float angle = player->getRotation() * PI / 180.0f;
        Vector2D direction = { cos(angle), sin(angle) };
        Vector2D bullet_pos = player->getPosition() + direction * 15.0f;
        Vector2D bullet_vel = direction * 400.0f;
        bullets.push_back(std::make_unique<Bullet>(bullet_pos, bullet_vel));
        shootCooldown = SHOOT_COOLDOWN_TIME;
        player->setShooting(false); // Reset shooting state to require a new key press
    }

    // Update all game objects
    player->update(deltaTime);
    for (auto& asteroid : asteroids) asteroid->update(deltaTime);
    for (auto& bullet : bullets) bullet->update(deltaTime);

    handleCollisions();

    // Remove inactive objects
    bullets.erase(std::remove_if(bullets.begin(), bullets.end(), [](const auto& b) { return !b->isActive(); }), bullets.end());
    asteroids.erase(std::remove_if(asteroids.begin(), asteroids.end(), [](const auto& a) { return !a->isActive(); }), asteroids.end());

    // Check for win condition
    if (asteroids.empty()) {
        // For simplicity, we just spawn more asteroids.
        spawnAsteroids(4 + (3 - player->getLives()));
    }
}

void Game::handleCollisions() {
    // Player-Asteroid collisions
    for (auto& asteroid : asteroids) {
        if (!player->isActive() || !asteroid->isActive()) continue;

        Vector2D diff = player->getPosition() - asteroid->getPosition();
        float dist = diff.magnitude();
        if (dist < player->getRadius() + asteroid->getRadius()) {
            player->loseLife();
            player->reset();
            if (player->getLives() <= 0) {
                gameState = GameState::GAME_OVER;
                player->destroy();
            }
            // In a real game, you might want a brief invulnerability period here.
        }
    }

    // Bullet-Asteroid collisions
    std::vector<std::unique_ptr<Asteroid>> new_asteroids;
    for (auto& bullet : bullets) {
        for (auto& asteroid : asteroids) {
            if (!bullet->isActive() || !asteroid->isActive()) continue;

            Vector2D diff = bullet->getPosition() - asteroid->getPosition();
            float dist = diff.magnitude();

            if (dist < bullet->getRadius() + asteroid->getRadius()) {
                bullet->destroy();
                asteroid->destroy();
                player->addScore(100); // Add score

                if (asteroid->getSize() == AsteroidSize::LARGE || asteroid->getSize() == AsteroidSize::MEDIUM) {
                    AsteroidSize new_size = (asteroid->getSize() == AsteroidSize::LARGE) ? AsteroidSize::MEDIUM : AsteroidSize::SMALL;
                    std::random_device rd;
                    std::mt19937 gen(rd());
                    std::uniform_real_distribution<> vel_dist(-70.0f, 70.0f);

                    for (int i = 0; i < 2; ++i) {
                        Vector2D new_vel = { (float)vel_dist(gen), (float)vel_dist(gen) };
                        new_asteroids.push_back(std::make_unique<Asteroid>(asteroid->getPosition(), new_vel, new_size));
                    }
                }
            }
        }
    }

    // Add the newly created asteroids from collisions
    for(auto& new_ast : new_asteroids) {
        asteroids.push_back(std::move(new_ast));
    }
}

void Game::render() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Black background
    SDL_RenderClear(renderer);

    // Render game objects
    if (player->isActive()) {
        player->render(renderer);
    }
    for (const auto& asteroid : asteroids) asteroid->render(renderer);
    for (const auto& bullet : bullets) bullet->render(renderer);

    // Render UI
    SDL_Color white = {255, 255, 255, 255};
    std::string scoreText = "Score: " + std::to_string(player->getScore());
    textRenderer->renderText(scoreText, 10, 10, white);

    std::string livesText = "Lives: " + std::to_string(player->getLives());
    textRenderer->renderText(livesText, SCREEN_WIDTH - 100, 10, white);


    if (gameState == GameState::GAME_OVER) {
        textRenderer->renderText("GAME OVER", SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2 - 50, white);
        textRenderer->renderText("Press 'R' to Restart", SCREEN_WIDTH / 2 - 150, SCREEN_HEIGHT / 2, white);
    }


    SDL_RenderPresent(renderer);
}