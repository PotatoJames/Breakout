#include "Paddle.h"
#include <iostream>
#include <SFML/Window/Mouse.hpp>

Paddle::Paddle(sf::RenderWindow* window)
    : _window(window), _width(PADDLE_WIDTH), _timeInNewSize(0.0f), _isAlive(true), _colorChangeTimer(0.0f)
{
    _sprite.setFillColor(sf::Color::Cyan);
    _sprite.setPosition((window->getSize().x - _width) / 2.0f, window->getSize().y - 50.0f);
    _sprite.setSize(sf::Vector2f(_width, PADDLE_HEIGHT));
}

Paddle::~Paddle()
{
}

void Paddle::moveLeft(float dt)
{
    float position = _sprite.getPosition().x;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) && position > 0)
    {
        _sprite.move(sf::Vector2f(-dt * PADDLE_SPEED, 0));
    }
}

void Paddle::moveRight(float dt)
{
    float position = _sprite.getPosition().x;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) && position < _window->getSize().x - _width)
    {
        _sprite.move(sf::Vector2f(dt * PADDLE_SPEED, 0));
    }
}

void Paddle::update(float dt)
{
    // Handle color reset
    if (_colorChangeTimer > 0)
    {
        _colorChangeTimer -= dt;
        if (_colorChangeTimer <= 0)
        {
            _sprite.setFillColor(sf::Color::White); // Reset to default color
        }
    }

    if (_timeInNewSize > 0)
    {
        _timeInNewSize -= dt;
    }
    else
    {
        setWidth(1.0f, 0.0f); // Reset to default width after duration
    }
}

void Paddle::render()
{
    _window->draw(_sprite);
}

sf::FloatRect Paddle::getBounds() const
{
    return _sprite.getGlobalBounds();
}

// update width by SF of coeff. 
// ensure centre remains consistent.
void Paddle::setWidth(float coeff, float duration)
{
    _width = coeff * PADDLE_WIDTH;
    _sprite.setSize(sf::Vector2f(_width, _sprite.getSize().y));
    _timeInNewSize = duration;
    float newX = _sprite.getPosition().x + (_width - PADDLE_WIDTH) / 2;
    _sprite.setPosition(newX, _sprite.getPosition().y);
}

void Paddle::trackMouse(const sf::RenderWindow& window)
{
    // Get the mouse's X position relative to the window
    sf::Vector2i mousePosition = sf::Mouse::getPosition(window);

    // Convert the mouse's X position to the paddle's position
    float newX = static_cast<float>(mousePosition.x) - (_sprite.getSize().x / 2);

    // Clamp the paddle's position to stay within the window bounds
    float windowWidth = static_cast<float>(window.getSize().x);
    newX = std::max(0.0f, std::min(newX, windowWidth - _sprite.getSize().x));

    // Update the paddle's position
    _sprite.setPosition(newX, _sprite.getPosition().y);
}

void Paddle::changeColorForASecond()
{
    _sprite.setFillColor(sf::Color::Red); // Change to red
    _colorChangeTimer = 1.0f; // Set timer to 1 second
}
