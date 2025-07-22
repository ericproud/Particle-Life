#pragma once
#include <SFML/Graphics.hpp>
#include <map>
#include <tuple>

//Defining window dimensions
const float SCREEN_HEIGHT = 1920.0f;
const float SCREEN_WIDTH = 1080.0f;

//Defining the area where particles are contained and displayed
const sf::Vector2f PARTICLE_AREA = {1316.0f, 1076.0f};
const sf::Vector2f PARTICLE_AREA_HALF = {658.0f, 538.0f};
const sf::Vector2f PARTICLE_AREA_TL= {602.0f, 2.0f};
const sf::Vector2f PARTICLE_AREA_BR= {1918.0f, 1078.0f};

//Unimportant to understand, this is used as a way to order sf::Color objects
//so they can be mapped in color_to_idx and idx_to_color
struct ColorLess {
    bool operator()(const sf::Color& a, const sf::Color& b) const {
        // lex order by r, g, b, a
        return std::tie(a.r, a.g, a.b, a.a) < std::tie(b.r, b.g, b.b, b.a);
    }
};

//Used to map colors to indices and vice versa, used often when working with the attraction matrix
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
