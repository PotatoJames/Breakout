#include "BrickManager.h"
#include "GameManager.h"

BrickManager::BrickManager(sf::RenderWindow* window, GameManager* gameManager)
    : _window(window), _gameManager(gameManager)
{
}

void BrickManager::createBricks(int rows, int cols, float brickWidth, float brickHeight, float spacing)
{
    float leftEdge;
    if (cols % 2 == 0) 
        leftEdge = _window->getSize().x / 2 - ((cols / 2.0f) * brickWidth + (cols / 2.0f - 0.5f) * spacing);
    else
        leftEdge = _window->getSize().x / 2 - ((cols / 2.0f - 0.5f) * brickWidth + (cols / 2.0f) * spacing);

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            float x = j * (brickWidth + spacing) + leftEdge;
            float y = i * (brickHeight + spacing) + TOP_PADDING;
            _bricks.emplace_back(x, y, brickWidth, brickHeight);
        }
    }
}

void BrickManager::render()
{
    for (auto& brick : _bricks) {
        brick.render(*_window);
    }
}

int BrickManager::checkCollision(sf::CircleShape& ball, sf::Vector2f& direction)
{
    int collisionResponse = 0;  // set to 1 for horizontal collision and 2 for vertical.
    for (auto& brick : _bricks) {
        if (!brick.getBounds().intersects(ball.getGlobalBounds())) continue;    // no collision, skip.

        sf::Vector2f ballPosition = ball.getPosition();
        float ballY = ballPosition.y + 0.5f * ball.getGlobalBounds().height;
        sf::FloatRect brickBounds = brick.getBounds();

        // default vertical bounce (collision is top/bottom)
        collisionResponse = 2;
        if (ballY > brickBounds.top && ballY < brickBounds.top + brickBounds.height)
            // unless it's horizontal (collision from side)
            collisionResponse = 1;

        sf::Vector2f brickPosition(brick.getBounds().left + brick.getBounds().width / 2,
                                   brick.getBounds().top + brick.getBounds().height / 2);
        spawnParticles(brickPosition);

        // Mark the brick as destroyed (for simplicity, let's just remove it from rendering)
        // In a complete implementation, you would set an _isDestroyed flag or remove it from the vector
        brick = _bricks.back();
        _bricks.pop_back();
        break;
    }
    if (_bricks.size() == 0)
    {
        _gameManager->levelComplete();
    }
    return collisionResponse;
}

void BrickManager::spawnParticles(sf::Vector2f position) 
{
    for (int i = 0; i < 10; ++i) {
        float angle = static_cast<float>(rand() % 360) * 3.14159f / 180.0f;
        sf::Vector2f velocity(std::cos(angle) * 100.0f, std::sin(angle) * 100.0f);
        _particles.emplace_back(position, velocity, 1.0f);
    }
}

void BrickManager::updateParticles(float dt) 
{
    for (auto it = _particles.begin(); it != _particles.end();) 
    {
        it->lifetime -= dt;
        if (it->lifetime <= 0) 
        {
            it = _particles.erase(it);
        } else {
            it->shape.move(it->velocity * dt);
            ++it;
        }
    }
}

void BrickManager::renderParticles(sf::RenderWindow& window) 
{
    for (const auto& particle : _particles) 
    {
        window.draw(particle.shape);
    }
}

void BrickManager::clearBricks() 
{
    _bricks.clear();
}