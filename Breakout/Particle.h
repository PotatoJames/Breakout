#pragma once
#include <SFML/Graphics.hpp>

struct Particle {
    sf::CircleShape shape;
    sf::Vector2f velocity;
    float lifetime;

    Particle(sf::Vector2f position, sf::Vector2f velocity, float lifetime)
        : velocity(velocity), lifetime(lifetime) {
        shape.setRadius(2.0f);
        shape.setPosition(position);
        shape.setFillColor(sf::Color::Yellow); 
    }
};