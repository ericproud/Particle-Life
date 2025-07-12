#pragma once
#include <SFML/Graphics.hpp>
#include "simulator.hpp"
#include <string>

class Displayer {
public:
    sf::CircleShape circle;
    sf::RectangleShape rect;
    sf::Font font{"assets/Swansea.ttf"};
    sf::Text text{font, "", 80};
    std::string string;

    explicit
    Displayer(sf::RenderTarget& target)
        : m_target{target}
    {
    }

    void drawUIBox(sf::Vector2f top_left, float width, float height) {
        rect.setPosition(top_left);
        rect.setSize(sf::Vector2f(width, height));
        rect.setOutlineColor(sf::Color(128, 128, 128));
        rect.setOutlineThickness(1);
        rect.setFillColor(sf::Color::Black);
        m_target.draw(rect);
    }

    void drawSquareBox(sf::Vector2f top_left, float size, bool fill_with_circle, sf::Color color) {
        rect.setOutlineThickness(0);
        if (!fill_with_circle) {
            rect.setPosition(top_left);
            rect.setSize(sf::Vector2f(size, size));
            rect.setFillColor(color);
            m_target.draw(rect);
        }
        circle.setRadius(size / 2);
        circle.setOrigin(sf::Vector2f(size / 2, size / 2));
        circle.setPosition(top_left + sf::Vector2f(size / 2, size / 2));
        circle.setFillColor(color);
        m_target.draw(circle);
    }

    void displayParticles(Simulator &sim) {
        drawUIBox({601, 1}, 1318, 1078);
        circle.setRadius(1.0f);
        circle.setPointCount(64);
        circle.setOrigin({1.0f, 1.0f});
        //CONTAINER
        for (auto &particle : sim.particles) {
            circle.setFillColor(particle.c);
            circle.setPosition(particle.pos);
            m_target.draw(circle);
        }
    }

    void displayAttractionModifier(Simulator &sim) {
        drawUIBox({1, 72}, 600, 544);

        text.setFont(font);
        text.setFillColor(sf::Color::White);
        text.setCharacterSize(30);

        string = "ATTRACTION MATRIX:";
        text.setString(string);
        text.setPosition({10, 72});
        m_target.draw(text);

        string = "RANDOMIZE\n\n\nPRESET MODE";
        text.setString(string);
        text.setPosition({10, 532});
        m_target.draw(text);

        sf::Vector2f top_left = sim.attraction_modifier.tl ;
        float box_size = 400.0f / (1.0f + static_cast<float>(sim.num_colors));

        for (int i{0}; i < sim.num_colors; i++) {
            drawSquareBox(top_left + sf::Vector2f((i + 1) * box_size, 0), box_size, true, idx_to_color[i]);
            drawSquareBox(top_left + sf::Vector2f(0, (i + 1) * box_size), box_size, true, idx_to_color[i]);
        }

        for (int i{0}; i < sim.num_colors; i++) {
            for (int j{0}; j < sim.num_colors; j++) {
                sf::Vector2f temp_tl = top_left + sf::Vector2f((i + 1) * box_size, (j + 1) * box_size);
                sf::Color color = sim.attraction_modifier.getColor(sim.attraction_modifier.attraction_matrix[i][j]);

                drawSquareBox(temp_tl, box_size, false, color);
            }
        }
    }

    void displayFPS(int event_fps, int physics_fps, int graphics_fps) {
        drawUIBox({1.0f, 1.0f}, 600, 70);

        text.setFont(font);
        text.setFillColor(sf::Color::White);
        text.setCharacterSize(30);

        string = "PHYSICS FPS: " + std::to_string(physics_fps) + "\n\n"
               + "GRAPHICS FPS: " + std::to_string(graphics_fps);
        text.setString(string);
        text.setPosition({10, 1});
        m_target.draw(text);
    }

    void displayParticleCountAndDistribution(Simulator &sim) {
        drawUIBox({1, 616}, 600, 290);
        int num_particles = sim.getParticleCount();
        text.setFont(font);
        text.setFillColor(sf::Color::White);
        text.setCharacterSize(30);

        string = "PARTICLE COUNT: " + std::to_string(num_particles) + "\n\nPARTICLE DISTRIBUTION:";
        text.setString(string);
        text.setPosition({10, 616});
        m_target.draw(text);

        m_target.draw(sim.increase_particles_button.button_zone);
        m_target.draw(sim.decrease_particles_button.button_zone);

        std::array<float, 7> color_dist = sim.getColorDistribution();
        sf::Vector2f top_left = {10, 700};

        rect.setOutlineThickness(1);

        for (int i{0}; i < 7; ++i) {
            float length = static_cast<int>(500 * color_dist[i]);
            rect.setFillColor(idx_to_color[i]);
            rect.setOutlineColor(idx_to_color[i]);
            rect.setPosition(top_left + sf::Vector2f(0, i * 20));
            printf("%f", rect.getPosition().x);
            rect.setSize({length, 20});
            m_target.draw(rect);
        }
        string = "EQUALIZE DISTRIBUTION\n\nRANDOMIZE DISTRIBUTION";
        text.setString(string);
        text.setPosition({10, 841});
        m_target.draw(text);
    }



    void displaySlider(Slider &slider) {

        m_target.draw(slider.slider_zone);
        m_target.draw(slider.knob_zone);
    }

    void displayParameterModifier(Simulator &sim) {
        drawUIBox({1, 906}, 600, 173);

        text.setFont(font);
        text.setFillColor(sf::Color::White);
        text.setCharacterSize(30);

        string = "BETA";
        text.setString(string);
        text.setPosition({10, 911});
        m_target.draw(text);

        displaySlider(sim.parameter_modifier.beta_slider);

        string = "D_MAX";
        text.setString(string);
        text.setPosition({10, 971});
        m_target.draw(text);

        displaySlider(sim.parameter_modifier.d_max_slider);

        string = "DT_HALF";
        text.setString(string);
        text.setPosition({10, 1031});
        m_target.draw(text);

        displaySlider(sim.parameter_modifier.dt_half_slider);
    }


private:
    sf::RenderTarget& m_target;
};