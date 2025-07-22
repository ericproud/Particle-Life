#pragma once
#include <SFML/Graphics.hpp>
#include "simulator.hpp"
#include "config.hpp"
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

    void drawUIBox(const sf::Vector2f top_left, const float width, const float height) {
        rect.setPosition(top_left);
        rect.setSize(sf::Vector2f(width, height));
        rect.setOutlineColor(sf::Color(128, 128, 128));
        rect.setOutlineThickness(1);
        rect.setFillColor(sf::Color::Black);
        m_target.draw(rect);
    }

    void drawSquareBox(const sf::Vector2f top_left, const float side_lens, const bool fill_with_circle, const sf::Color color) {
        rect.setOutlineThickness(0);
        if (!fill_with_circle) {
            rect.setPosition(top_left);
            rect.setSize(sf::Vector2f(side_lens, side_lens));
            rect.setFillColor(color);
            m_target.draw(rect);
        }
        circle.setRadius(side_lens / 2);
        circle.setOrigin(sf::Vector2f(side_lens / 2, side_lens / 2));
        circle.setPosition(top_left + sf::Vector2f(side_lens / 2, side_lens / 2));
        circle.setFillColor(color);
        m_target.draw(circle);
    }

    void drawSlider(const Slider &slider) const {
        m_target.draw(slider.slider_zone);
        m_target.draw(slider.knob_zone);
    }


    void displayParticles(const Simulator &sim) {
        //Drawing container
        drawUIBox({601, 1}, 1318, 1078);
        circle.setRadius(1.0f);
        circle.setPointCount(4);
        circle.setOrigin({1.0f, 1.0f});

        //Drawing particles
        sf::VertexArray points(sf::PrimitiveType::Points, sim.particles.size());

        for (int i = 0; i < sim.particles.size(); ++i) {
            points[i].position = sim.particles[i].pos;
            points[i].color = sim.particles[i].c;
        }
        m_target.draw(points);
    }

    void displayAttractionModifier(Simulator &sim) {
        //Drawing Container
        drawUIBox({1, 72}, 600, 544);

        //Boilerplate
        circle.setPointCount(32);
        text.setFont(font);
        text.setFillColor(sf::Color::White);
        text.setCharacterSize(30);

        //Drawing text descriptors
        string = "ATTRACTION MATRIX:";
        text.setString(string);
        text.setPosition({10, 72});
        m_target.draw(text);

        string = "RESET";
        text.setString(string);
        text.setPosition({10, 530});
        m_target.draw(text);
        m_target.draw(sim.reset_matrix_button.button_zone);

        string = "RANDOMIZE";
        text.setString(string);
        text.setPosition({10, 565});
        m_target.draw(text);
        m_target.draw(sim.randomize_matrix_button.button_zone);


        string = "WORM MODE";
        text.setString(string);
        text.setPosition({275, 530});
        m_target.draw(text);
        m_target.draw(sim.worm_mode_button.button_zone);

        string = "COLOR COUNT";
        text.setString(string);
        text.setPosition({275, 565});
        m_target.draw(text);
        m_target.draw(sim.increase_color_count_button.button_zone);
        m_target.draw(sim.decrease_color_count_button.button_zone);

        //Drawing the attraction matrix (buttons)
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

                //Drawing button
                drawSquareBox(temp_tl, box_size, false, color);
            }
        }
    }

    void displayFPS(const int physics_fps, const int graphics_fps) {
        //Drawing container
        drawUIBox({1.0f, 1.0f}, 600, 70);

        //Boilerplate
        text.setFont(font);
        text.setFillColor(sf::Color::White);
        text.setCharacterSize(30);

        //Drawing text descriptors
        string = "PHYSICS FPS: " + std::to_string(physics_fps) + "\n\n"
               + "GRAPHICS FPS: " + std::to_string(graphics_fps);
        text.setString(string);
        text.setPosition({10, 1});
        m_target.draw(text);
    }

    void displayParticleCountAndDistribution(const Simulator &sim) {
        //Drawing container
        drawUIBox({1, 616}, 600, 290);

        //Boilerplate
        int num_particles = sim.getParticleCount();
        text.setFont(font);
        text.setFillColor(sf::Color::White);
        text.setCharacterSize(30);

        //Drawing text desctiptor
        string = "PARTICLE COUNT: " + std::to_string(num_particles) + "\n\nPARTICLE DISTRIBUTION:";
        text.setString(string);
        text.setPosition({10, 616});
        m_target.draw(text);

        //Drawing buttons
        m_target.draw(sim.increase_particles_button.button_zone);
        m_target.draw(sim.decrease_particles_button.button_zone);

        //Drawing color distribution
        std::array<float, 7> color_dist = sim.getColorDistribution();
        sf::Vector2f top_left = {10, 685};

        for (int i{0}; i < 7; ++i) {
            float length = static_cast<int>(500 * color_dist[i]);
            rect.setFillColor(idx_to_color[i]);
            rect.setOutlineColor(idx_to_color[i]);
            rect.setPosition(top_left + sf::Vector2f(0, i * 20));
            rect.setSize({length, 20});
            m_target.draw(rect);
        }

        //Drawing text descriptors
        string = "EQUALIZE DISTRIBUTION";
        text.setString(string);
        text.setPosition({10, 826});
        m_target.draw(text);

        string = "RANDOMIZE DISTRIBUTION";
        text.setString(string);
        text.setPosition({10, 866});
        m_target.draw(text);

        //Drawing buttons
        m_target.draw(sim.equalize_distribution_button.button_zone);
        m_target.draw(sim.randomize_distribution_button.button_zone);
    }

    void displayParameterModifier(Simulator &sim) {
        //Drawing container
        drawUIBox({1, 906}, 600, 173);

        //Boilerplate
        text.setFont(font);
        text.setFillColor(sf::Color::White);
        text.setCharacterSize(30);

        //Drawing text descriptors
        string = "BETA";
        text.setString(string);
        text.setPosition({10, 911});
        m_target.draw(text);

        string = "D_MAX";
        text.setString(string);
        text.setPosition({10, 971});
        m_target.draw(text);

        string = "DT_HALF";
        text.setString(string);
        text.setPosition({10, 1031});
        m_target.draw(text);

        //Drawing Sliders
        drawSlider(sim.parameter_modifier.beta_slider);
        drawSlider(sim.parameter_modifier.d_max_slider);
        drawSlider(sim.parameter_modifier.dt_half_slider);

        //Drawing knob (beta slider)
        sf::Vector2f temp_pos = sim.parameter_modifier.beta_slider.knob_zone.getPosition() - sf::Vector2f{15, 15};
        string = std::to_string(sim.parameter_modifier.roundToDecimalPlaces(sim.parameter_modifier.beta_slider.getValue(), 2));
        string = string.substr(1, 3);
        text.setString(string);
        text.setPosition(temp_pos);
        m_target.draw(text);

        //Drawing knob (d_max slider)
        temp_pos = sim.parameter_modifier.d_max_slider.knob_zone.getPosition() - sf::Vector2f{15, 15};
        string = std::to_string(static_cast<int>(sim.parameter_modifier.roundToDecimalPlaces(sim.parameter_modifier.d_max_slider.getValue(), 2)));
        string = string.substr(0, 2);
        text.setString(string);
        text.setPosition(temp_pos);
        m_target.draw(text);

        //Drawing knob (dt_half slider)
        temp_pos = sim.parameter_modifier.dt_half_slider.knob_zone.getPosition() - sf::Vector2f{15, 15};
        string = std::to_string(sim.parameter_modifier.roundToDecimalPlaces(sim.parameter_modifier.dt_half_slider.getValue(), 3));
        string = string.substr(1, 4);
        text.setString(string);
        text.setPosition(temp_pos);
        m_target.draw(text);
    }

private:
    sf::RenderTarget& m_target;
};