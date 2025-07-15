#include <SFML/Graphics.hpp>
#include "simulator.hpp"
#include "displayer.hpp"
#include <iostream>

int main()
{

    std::cout << "Running with " << omp_get_max_threads() << " threads\n";

    sf::VideoMode desktopMode = sf::VideoMode::getDesktopMode();
    sf::RenderWindow window(desktopMode, "Particle Life", sf::Style::Default,  sf::State::Fullscreen);
    window.setFramerateLimit(60);

    Simulator sim;
    Displayer displayer(window);
    sf::Clock clock;

    int event_fps = 0;
    int physics_fps = 0;
    int graphics_fps = 0;

    while (window.isOpen())
    {
        //EVENT POLLING WORK
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }
            if (const auto* mouseButtonPressed = event->getIf<sf::Event::MouseButtonPressed>()) {
                if (mouseButtonPressed->button == sf::Mouse::Button::Left) {
                    auto click_pos = static_cast<sf::Vector2f>(mouseButtonPressed->position);

                    //HANDLING MATRIX INTERACTIONS
                    for (int i{0}; i < 7; ++i) {
                        for (int j{0}; j < 7; ++j) {
                            if (sim.attraction_modifier.button_matrix[i][j].isPressed(click_pos)) {
                                sim.attraction_modifier.updateButton(i, j);
                                sim.is_worm_mode = false;
                                sim.worm_mode_button.button_zone.setFillColor(sf::Color::Red);
                            }
                        }
                    }
                    if (sim.increase_color_count_button.isPressed(click_pos)) {
                        if (sim.num_colors < 7) {
                            sim.setNumColors(sim.num_colors + 1);
                        }
                        else {
                            sim.setNumColors(1);
                        }
                        sim.evenlyDiststributeParticleColors();
                        if (sim.is_worm_mode) {
                            sim.attraction_modifier.wormAttractinMatrix(sim.num_colors);
                        }
                    }
                    if (sim.decrease_color_count_button.isPressed(click_pos)) {
                        if (sim.num_colors > 1) {
                            sim.setNumColors(sim.num_colors - 1);
                        }
                        else {
                            sim.setNumColors(7);
                        }
                        sim.evenlyDiststributeParticleColors();
                        if (sim.is_worm_mode) {
                            sim.attraction_modifier.wormAttractinMatrix(sim.num_colors);
                        }
                    }
                    if (sim.reset_matrix_button.isPressed(click_pos)) {
                        sim.attraction_modifier.zeroAttractionMatrix();
                        sim.is_worm_mode = false;
                        sim.worm_mode_button.button_zone.setFillColor(sf::Color::Red);
                    }
                    if (sim.randomize_matrix_button.isPressed(click_pos)) {
                        sim.attraction_modifier.randomizeAttractionMatrix();
                        sim.is_worm_mode = false;
                        sim.worm_mode_button.button_zone.setFillColor(sf::Color::Red);
                    }
                    if (sim.worm_mode_button.isPressed(click_pos)) {
                        sim.attraction_modifier.wormAttractinMatrix(sim.num_colors);
                        sim.is_worm_mode = true;
                        sim.worm_mode_button.button_zone.setFillColor(sf::Color::Green);
                    }

                    //HANDLING PARTICLE COUNT/COLOR INTERACTIONS
                    if (sim.increase_particles_button.isPressed(click_pos)) {
                        sim.insertParticlesEvenDistribution(500, sim.num_colors);
                    }
                    if (sim.decrease_particles_button.isPressed(click_pos)) {
                        sim.removeParticles(500);
                    }
                    if (sim.randomize_distribution_button.isPressed(click_pos)) {
                        sim.randomlyDistributeParticleColors();
                    }
                    if (sim.equalize_distribution_button.isPressed(click_pos)) {
                        sim.evenlyDiststributeParticleColors();
                    }

                    //HANDLING PARAMETER INTERACTIONS
                    if (sim.parameter_modifier.beta_slider.isPressed(click_pos)) {
                        sim.parameter_modifier.beta_slider.setKnobPos(click_pos);
                        sim.BETA =   sim.roundToDecimalPlaces(sim.parameter_modifier.beta_slider.getValue(), 2);
                    }
                    if (sim.parameter_modifier.d_max_slider.isPressed(click_pos)) {
                        sim.parameter_modifier.d_max_slider.setKnobPos(click_pos);
                        sim.D_MAX = sim.roundToDecimalPlaces(sim.parameter_modifier.d_max_slider.getValue(), 2);
                        sim.D_MAX_updated = true;
                    }
                    if (sim.parameter_modifier.dt_half_slider.isPressed(click_pos)) {
                        sim.parameter_modifier.dt_half_slider.setKnobPos(click_pos);
                        sim.DT_HALF = sim.roundToDecimalPlaces(sim.parameter_modifier.dt_half_slider.getValue(),3);
                    }
                }
            }
        }

        event_fps = sim.getFPS(clock);

        //PHYSICS WORK
        sim.buildPartition();
        sim.tick();

        physics_fps = sim.getFPS(clock);

        //GRAPHICS WORK
        window.clear();
        displayer.displayParticles(sim);
        displayer.displayAttractionModifier(sim);
        displayer.displayFPS(event_fps, physics_fps, graphics_fps);
        displayer.displayParticleCountAndDistribution(sim);
        displayer.displayParameterModifier(sim);
        window.display();

        graphics_fps = sim.getFPS(clock);
    }
    return 0;
}
