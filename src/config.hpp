#pragma once
#include <SFML/Graphics.hpp>
#include <map>
#include <tuple>

const int SCREEN_HEIGHT = 1920;
const int SCREEN_WIDTH = 1080;

const sf::Vector2f PARTICLE_AREA_TOP_LEFT = {600, 0};
const sf::Vector2i PARTICLE_AREA_SIDE_LENGTHS = {1316, 1076};

struct ColorLess {
    bool operator()(const sf::Color& a, const sf::Color& b) const {
        // lex order by r, g, b, a
        return std::tie(a.r, a.g, a.b, a.a) < std::tie(b.r, b.g, b.b, b.a);
    }
};

inline std::map<sf::Color, int, ColorLess> color_to_idx = {
                                                    {sf::Color::White, 0},
                                                    {sf::Color::Red, 1},
                                                    {sf::Color::Green, 2},
                                                    {sf::Color::Blue, 3},
                                                    {sf::Color::Yellow, 4},
                                                    {sf::Color::Magenta, 5},
                                                    {sf::Color::Cyan, 6}
                                                  };

inline std::map<int, sf::Color> idx_to_color = {
                                                    {0, sf::Color::White},
                                                    {1, sf::Color::Red},
                                                    {2, sf::Color::Green},
                                                    {3, sf::Color::Blue},
                                                    {4, sf::Color::Yellow},
                                                    {5, sf::Color::Magenta},
                                                    {6, sf::Color::Cyan}
                                                  };
