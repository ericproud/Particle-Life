#pragma once
#include <SFML/Graphics.hpp>
#include "config.hpp"

//Defining the particle strut
struct Particle {
    sf::Vector2f pos;
    sf::Vector2f vel;
    sf::Color c;
    int c_idx;

    Particle(float x, float y, sf::Color c_): pos{x, y}, vel{0.0f, 0.0f}, c{c_}, c_idx{color_to_idx[c_]} {}
};