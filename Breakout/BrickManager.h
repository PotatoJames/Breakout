#pragma once
#include <vector>
#include "Brick.h"
#include <SFML/Graphics.hpp>
#include "Particle.h"

class GameManager;

class BrickManager {
public:
    BrickManager(sf::RenderWindow* window, GameManager* gameManager);
    void createBricks(int rows, int cols, float brickWidth, float brickHeight, float spacing);
    void render();
    int checkCollision(sf::CircleShape& ball, sf::Vector2f& direction);
    void spawnParticles(sf::Vector2f position);
    void updateParticles(float dt);
    void renderParticles(sf::RenderWindow& window);

private:
    std::vector<Brick> _bricks;
    std::vector<Particle> _particles; // Particle system
    sf::RenderWindow* _window;

    GameManager* _gameManager;
    static constexpr float TOP_PADDING = 100.0f;
};
