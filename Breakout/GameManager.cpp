#include "GameManager.h"
#include "Ball.h"
#include "PowerupManager.h"
#include <iostream>

constexpr int POWERUP_SPAWN_CHANCE = 700;
constexpr float POWERUP_MIN_INTERVAL = 7.5f;

GameManager::GameManager(sf::RenderWindow* window)
    : _window(window), _paddle(nullptr), _ball(nullptr), _brickManager(nullptr), _powerupManager(nullptr),
      _messagingSystem(nullptr), _ui(nullptr), _pause(false), _time(0.f), _lives(3), _pauseHold(0.f), _levelComplete(false),
      _powerupInEffect({ none, 0.f }), _timeLastPowerupSpawned(0.f), _useMouseInput(true)
{
    _font.loadFromFile("font/montS.ttf");
    _masterText.setFont(_font);
    _masterText.setPosition(50, 400);
    _masterText.setCharacterSize(48);
    _masterText.setFillColor(sf::Color::Yellow);
    _masterText.setString("Mouse enabled. Press SPACE to switch.");
}

void GameManager::initialize()
{
    _paddle = new Paddle(_window);
    _brickManager = new BrickManager(_window, this);
    _messagingSystem = new MessagingSystem(_window);
    _ball = new Ball(_window, 400.0f, this); 
    _powerupManager = new PowerupManager(_window, _paddle, _ball);
    _ui = new UI(_window, _lives, this);

    // Create bricks
    _brickManager->createBricks(5, 10, 80.0f, 30.0f, 5.0f);
}

void GameManager::update(float dt)
{
    _powerupInEffect = _powerupManager->getPowerupInEffect();
    _ui->updatePowerupText(_powerupInEffect);
    _powerupInEffect.second -= dt;

    
    if (_lives <= 0)
    {
        _masterText.setString("Game over. Press R to restart.");

        
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::R))
        {
            reset();
        }
        return;
    }

    // Check if the level is completed
    if (_levelComplete)
    {
        // Allow resetting the game by pressing 'R'
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::R))
        {
            reset(); // Reset the game state
        }
        return; // Skip the rest of the update logic
    }

    // Pause handling
    if (_pauseHold > 0.f) _pauseHold -= dt;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::P))
    {
        if (!_pause && _pauseHold <= 0.f)
        {
            _pause = true;
            _masterText.setString("paused.");
            _pauseHold = PAUSE_TIME_BUFFER;
        }
        if (_pause && _pauseHold <= 0.f)
        {
            _pause = false;
            _masterText.setString("");
            _pauseHold = PAUSE_TIME_BUFFER;
        }
    }
    if (_pause)
    {
        return;
    }

    // timer
    _time += dt;

    if (_time > _timeLastPowerupSpawned + POWERUP_MIN_INTERVAL && rand() % POWERUP_SPAWN_CHANCE == 0)
    {
        _powerupManager->spawnPowerup();
        _timeLastPowerupSpawned = _time;
    }

    
    static bool spaceHeld = false;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
    {
        if (!spaceHeld)
        {
            _useMouseInput = !_useMouseInput;
            if (_useMouseInput)
                _masterText.setString("Mouse enabled. Press SPACE to switch.");
            else
                _masterText.setString("Keyboard enabled. Press SPACE to switch.");
            spaceHeld = true;
        }
    }
    else
    {
        spaceHeld = false;
    }

    
    if (_useMouseInput)
    {
        _paddle->trackMouse(*_window);
    }
    else
    {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) _paddle->moveRight(dt);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) _paddle->moveLeft(dt);
    }

    // Update everything
    _paddle->update(dt);
    _ball->update(dt);
    _powerupManager->update(dt);
    _brickManager->updateParticles(dt);
}

void GameManager::loseLife()
{
    _lives--;
    _ui->lifeLost(_lives);

    // TODO screen shake.
}

void GameManager::render()
{
    _paddle->render();
    _ball->render();
    _brickManager->render();
    _powerupManager->render();
    _brickManager->renderParticles(*_window);
    _window->draw(_masterText);
    _ui->render();
}

void GameManager::levelComplete()
{
    _levelComplete = true;
}

void GameManager::reset()
{
    _lives = 3;
    _levelComplete = false;
    _time = 0.0f;
    _timeLastPowerupSpawned = 0.0f;

    _useMouseInput = true;
    _masterText.setString("Mouse enabled. Press SPACE to switch.");


    _brickManager->clearBricks();
    _brickManager->createBricks(5, 10, 80.0f, 30.0f, 5.0f);
    _powerupManager->update(0.0f);
    _ui->resetLives(_lives);
}

sf::RenderWindow* GameManager::getWindow() const { return _window; }
UI* GameManager::getUI() const { return _ui; }
Paddle* GameManager::getPaddle() const { return _paddle; }
BrickManager* GameManager::getBrickManager() const { return _brickManager; }
PowerupManager* GameManager::getPowerupManager() const { return _powerupManager; }
