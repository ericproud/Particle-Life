#pragma once
#include <SFML/Graphics.hpp>
#include "config.hpp"

struct Particle {
    sf::Vector2f pos, vel;
    sf::Color c;
    int c_idx;

    Particle(float x, float y, sf::Color c_): pos{x, y}, vel{0, 0}, c(c_), c_idx{color_to_idx[c_]} {}
};
//Test