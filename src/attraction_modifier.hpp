#pragma once
#include "SFML/Graphics/RectangleShape.hpp"
#include "SFML/System/Vector2.hpp"

struct Button {
    sf::RectangleShape button_zone;

    Button() = default;
    Button(sf::Vector2f top_left, sf::Vector2f side_lengths) {
        button_zone.setPosition(top_left);
        button_zone.setSize(side_lengths);
        button_zone.setFillColor(sf::Color(255, 255, 0));
    }

    bool isPressed(sf::Vector2f click_pos) {
        return button_zone.getGlobalBounds().contains(click_pos);
    }
};

class AttractionModifier {
public:
    float attraction_matrix[7][7];
    Button button_matrix[7][7];
    sf::Vector2f tl;
    sf::Vector2f side_lens;

    AttractionModifier(sf::Vector2f top_left, int num_types) {

        //Initialize buttons
        float box_size = 300.0f / (1.0f + static_cast<float>(num_types));

        for (int x{0}; x < num_types; ++x) {
            for (int y{0}; y < num_types; ++y) {
                sf::Vector2f temp_pos = {top_left.x + (x + 1) * box_size, top_left.y + (y + 1) * box_size};
                button_matrix[x][y] = Button(temp_pos, {box_size, box_size});
                attraction_matrix[x][y] = 0;
            }
        }
        for (int x{num_types}; x < 7; ++x) {
            for (int y{num_types}; y < 7; ++y) {
                button_matrix[x][y] = Button({2000, 2000}, {1, 1});
            }
        }
        tl = top_left;
    }

    void incrementAttractionMatrix(int row, int col) {
        attraction_matrix[row][col] += 0.1f;

        if (attraction_matrix[row][col] > 1.0f) {
            attraction_matrix[row][col] = -1.0f;
        }
    }

    sf::Color getColor(float value) {
        if (value < 0.0f) {
            value = value + 1.0f;
            const int extra_green = static_cast<int>(255.0f * value);
            return sf::Color(255, extra_green, 0);
        }
        //Else if value >= 0
        const int extra_red = static_cast<int>(255.0f * (1.0f - value));
        return sf::Color(extra_red, 255, 0);
    }

    void updateButton(int row, int col) {
        incrementAttractionMatrix(row, col);
        button_matrix[row][col].button_zone.setFillColor(getColor(attraction_matrix[row][col]));
    }

    void setButton(int row, int col, float val) {
        attraction_matrix[row][col] = val;
        button_matrix[row][col].button_zone.setFillColor(getColor(attraction_matrix[row][col]));
    }

    void changeButtonMatrix(int num_types) {
        float box_size = 300.0f / (1.0f + static_cast<float>(num_types));
        for (int x{0}; x < num_types; ++x) {
            for (int y{0}; y < num_types; ++y) {
                sf::Vector2f temp_pos = {tl.x + (x + 1) * box_size, tl.y + (y + 1) * box_size};
                button_matrix[x][y] = Button(temp_pos, {box_size, box_size});
            }
        }
        for (int x{num_types}; x < 7; ++x) {
            for (int y{num_types}; y < 7; ++y) {
                button_matrix[x][y] = Button({2000, 2000}, {1, 1});
            }
        }
    }

    void randomizeAttractionMatrix() {
        for (int i{0}; i < 7; ++i) {
            for (int j{0}; j < 7; ++j) {
                float random_float = static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX) * std::pow(-1, std::rand() % 2);
                attraction_matrix[i][j] = random_float;
                button_matrix[i][j].button_zone.setFillColor(getColor(attraction_matrix[i][j]));
            }
        }
    }

    void zeroAttractionMatrix() {
        for (int i{0}; i < 7; ++i) {
            for (int j{0}; j < 7; ++j) {
                setButton(i, j, 0);
            }
        }
    }

    void snakeAttractinMatrix(int num_colors) {
        for (int i{0}; i < num_colors; ++i) {
            for (int j{0}; j < num_colors; ++j) {
                if (i == j) {
                    attraction_matrix[i][j] = 1;
                }
                if (i == (j + 1)) {
                    attraction_matrix[i][j] = 0.5;
                }
                attraction_matrix[0][num_colors - 1] = 0.5;
            }
        }
    }

};