#include <SFML/Graphics.hpp>
#include "simulator.hpp"
#include "displayer.hpp"

int main()
{
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
            //HANDLING ATTRACTION MATRIX BUTTON PRESSES
            if (const auto* mouseButtonPressed = event->getIf<sf::Event::MouseButtonPressed>()) {
                if (mouseButtonPressed->button == sf::Mouse::Button::Left) {
                    auto click_pos = static_cast<sf::Vector2f>(mouseButtonPressed->position);
                    for (int i{0}; i < 7; ++i) {
                        for (int j{0}; j < 7; ++j) {
                            if (sim.attraction_modifier.button_matrix[i][j].isPressed(click_pos)) {
                                sim.attraction_modifier.updateButton(i, j);
                            }
                        }
                    }
                    if (sim.increase_particles_button.isPressed(click_pos)) {
                        sim.insertParticlesEvenDistribution(500, sim.num_colors);
                    }
                    if (sim.decrease_particles_button.isPressed(click_pos)) {
                        sim.removeParticles(500);
                    }

                }
            }
            //RESETTING ATTRACTION MATRIX TO ZERO'S
            if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
                if (keyPressed->code == sf::Keyboard::Key::Z) {
                    sim.attraction_modifier.zeroAttractionMatrix();
                }
            }
            //RANDOMIZING ATTRACTION MATRIX
            if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
                if (keyPressed->code == sf::Keyboard::Key::R) {
                    sim.attraction_modifier.randomizeAttractionMatrix();
                }
            }
            //INCREASING/DECREASING NUMBER OF COLORS
            if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
                if (keyPressed->code == sf::Keyboard::Key::P) {
                    if (sim.num_colors < 7) {
                        sim.setNumColors(sim.num_colors + 1);
                    }
                    else {
                        sim.setNumColors(1);
                    }

                    sim.attraction_modifier.changeButtonMatrix(sim.num_colors);
                    sim.evenlyDiststributeParticleColors();

                    if (sim.matrix_mode == 3) {
                        sim.attraction_modifier.zeroAttractionMatrix();
                        sim.attraction_modifier.snakeAttractinMatrix(sim.num_colors);
                    }
                }
                if (keyPressed->code == sf::Keyboard::Key::O) {
                    if (sim.num_colors > 1) {
                        sim.setNumColors(sim.num_colors - 1);
                    }
                    else {
                        sim.setNumColors(7);
                    }
                    sim.attraction_modifier.changeButtonMatrix(sim.num_colors);
                    sim.evenlyDiststributeParticleColors();

                    if (sim.matrix_mode == 3) {
                        sim.attraction_modifier.zeroAttractionMatrix();
                        sim.attraction_modifier.snakeAttractinMatrix(sim.num_colors);
                    }
                }
                if (keyPressed->code == sf::Keyboard::Key::D) {
                    sim.evenlyDiststributeParticleColors();
                }
                if (keyPressed->code == sf::Keyboard::Key::S) {
                    sim.attraction_modifier.snakeAttractinMatrix(sim.num_colors);
                    sim.matrix_mode = 3;
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
        displayer.displayParticleCount(sim);
        window.display();

        graphics_fps = sim.getFPS(clock);
    }
    return 0;
}
