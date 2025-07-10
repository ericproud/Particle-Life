#pragma once
#include <SFML/Graphics.hpp>
#include "simulator.hpp"
#include <string>

class Displayer {
public:
    sf::CircleShape circle;
    sf::RectangleShape rect;
    sf::Font font{"assets/Swansea.ttf"};
    sf::Text text{font, "", 20};
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
        drawUIBox({1, 61}, 400, 350);

        sf::Vector2f top_left = sim.attraction_modifier.tl;
        float box_size = 300.0f / (1.0f + static_cast<float>(sim.num_colors));

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
        drawUIBox({1.0f, 1.0f}, 400, 60);

        text.setFont(font);
        text.setFillColor(sf::Color::White);
        text.setCharacterSize(20);

        string = "Physics FPS: " + std::to_string(physics_fps);
        text.setString(string);
        text.setPosition({1, 1});
        m_target.draw(text);

        string = "GRAPHICS FPS: " + std::to_string(graphics_fps);
        text.setString(string);
        text.setPosition({1, 21});
        m_target.draw(text);

        string = "EVENTS FPS: " + std::to_string(event_fps);
        text.setString(string);
        text.setPosition({1, 41});
        m_target.draw(text);
    }

    void displayParticleCount(Simulator &sim) {
        int num_particles = sim.getParticleCount();
        text.setFont(font);
        text.setFillColor(sf::Color::White);
        text.setCharacterSize(20);
        string = "Number of particles: " + std::to_string(num_particles);
        text.setString(string);
        text.setPosition({1, 800});
        m_target.draw(text);

        m_target.draw(sim.increase_particles_button.button_zone);
        m_target.draw(sim.decrease_particles_button.button_zone);
    }



private:
    sf::RenderTarget& m_target;
};