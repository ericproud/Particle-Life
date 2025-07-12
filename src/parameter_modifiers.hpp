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

struct Slider {
    sf::RectangleShape slider_zone;
    sf::RectangleShape knob_zone;
    float slider_len;
    float knob_pos;
    float min_val;
    float max_val;

    Slider(sf::Vector2f top_left, sf::Vector2f side_lengths, float min_value, float max_value, float default_value):
    min_val(min_value),
    max_val(max_value),
    slider_len(side_lengths.x) {
        slider_zone.setPosition(top_left);
        slider_zone.setSize(side_lengths);
        slider_zone.setFillColor(sf::Color(127, 127, 127));

        knob_zone.setSize({side_lengths.y, side_lengths.y});
        knob_zone.setOrigin({side_lengths.y / 2, side_lengths.y / 2});
        knob_zone.setFillColor(sf::Color::Red);
        knob_zone.setOutlineColor(sf::Color::Red);

        setDefaultKnobPos(default_value);
        knob_zone.setPosition({knob_zone.getPosition().x, top_left.y + side_lengths.y / 2});
    }

    bool isPressed(sf::Vector2f click_pos) {
        return slider_zone.getGlobalBounds().contains(click_pos);
    }

    void setKnobPos(sf::Vector2f click_pos) {
        knob_pos = click_pos.x;
        knob_zone.setPosition({knob_pos, knob_zone.getPosition().y});
    }

    void setDefaultKnobPos(float default_val) {
        float range = max_val - min_val;
        float delta = default_val - min_val;
        knob_pos = slider_zone.getPosition().x + delta / range * slider_len;
        knob_zone.setPosition({knob_pos, knob_zone.getPosition().y});
    }

    float getValue() {
        float relative_slide = knob_pos - slider_zone.getPosition().x;
        float slide_pct = relative_slide / slider_len;
        return min_val + slide_pct * (max_val - min_val);
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
        float box_size = 400.0f / (1.0f + static_cast<float>(num_types));

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
        float box_size = 400.0f / (1.0f + static_cast<float>(num_types));
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

class ParameterModifier {
public:
    Slider beta_slider;
    Slider d_max_slider;
    Slider dt_half_slider;
    float BETA;
    float D_MAX;
    float DT_HALF;
    ParameterModifier(sf::Vector2f tl_BETA, sf::Vector2f tl_D_MAX, sf::Vector2f tl_DT_HALF):
    beta_slider(tl_BETA, {400, 40}, 0.1, 1, 0.3),
    d_max_slider(tl_D_MAX, {400, 40}, 2, 40, 40),
    dt_half_slider(tl_DT_HALF, {400, 40}, 0.004f, 0.08f, 0.04f),
    BETA(0.3f),
    D_MAX(40),
    DT_HALF(0.4f)
    {}

    float roundToDecimalPlaces(float value, int places) {
        float scale = std::pow(10.0f, places);
        return std::round(value * scale) / scale;
    }

    void updateParameters() {
        BETA =   roundToDecimalPlaces(beta_slider.getValue(), 2);
        printf("BETA: %f\n", BETA);
        D_MAX = roundToDecimalPlaces(d_max_slider.getValue(), 1);
        printf("D_MAX: %f\n", D_MAX);
        DT_HALF = roundToDecimalPlaces(dt_half_slider.getValue(),2);
        printf("DT_HALF: %f\n", DT_HALF);
    }
};