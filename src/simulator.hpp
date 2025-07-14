#pragma once
#include <cmath>
#include <vector>
#include "particle.hpp"
#include "config.hpp"
#include "parameter_modifiers.hpp"
#include <omp.h>

//const int SCREEN_HEIGHT = 1920;
//const int SCREEN_WIDTH = 1080;

struct Simulator {
    std::vector<Particle> particles;
    std::vector<std::vector<std::vector<Particle*>>> partition;
    AttractionModifier attraction_modifier;
    ParameterModifier parameter_modifier;
    Button increase_particles_button;
    Button decrease_particles_button;
    Button increase_color_count_button;
    Button decrease_color_count_button;
    Button reset_matrix_button;
    Button randomize_matrix_button;
    Button worm_mode_button;
    Button equalize_distribution_button;
    Button randomize_distribution_button;
    int num_colors;
    float BETA;
    float D_MAX;
    float D_MAX_INV;
    float DT;
    float DT_HALF;
    float DAMPING_FACTOR;
    bool is_worm_mode = false;
    bool D_MAX_updated = false;
    //

    Simulator():
        particles(),
        partition(),
        parameter_modifier({180, 911}, {180, 971}, {180, 1031}),
        attraction_modifier({101.0f, 121.0f}, 2),
        increase_particles_button({560, 620}, {30.0f, 30.0f}),
        decrease_particles_button({510, 620}, {30.0f, 30.0f}),
        increase_color_count_button({560, 565}, {30.0f, 30.0f}),
        decrease_color_count_button({510, 565}, {30.0f, 30.0f}),
        reset_matrix_button({200, 530}, {30.0f, 30.0f}),
        randomize_matrix_button({200, 565}, {30.0f, 30.0f}),
        worm_mode_button({510, 530}, {30.0f, 30.0f}),
        equalize_distribution_button({510, 826}, {30.0f, 30.0f}),
        randomize_distribution_button({510, 866}, {30.0f, 30.0f}),
        num_colors(2),
        BETA(0.3f),
        D_MAX(40),
        DT(0.02f),
        DT_HALF(0.04f)
    {
        particles.reserve(50000);
        int x_partitions = 1320 / static_cast<int>(D_MAX);
        int y_partitions = 1080 / static_cast<int>(D_MAX);

        partition.resize(x_partitions);
        for (auto& col : partition) {
            col.resize(y_partitions);
            for (auto& cell : col) {
                cell.reserve(100);
            }
        }

        insertParticlesEvenDistribution(10, 2);
        increase_particles_button.button_zone.setFillColor(sf::Color::Green);
        decrease_particles_button.button_zone.setFillColor(sf::Color::Red);
        increase_color_count_button.button_zone.setFillColor(sf::Color::Green);
        decrease_color_count_button.button_zone.setFillColor(sf::Color::Red);
    }

    void insertParticlesEvenDistribution(int num_particles, int num_colors) {
        for (int i = 0; i < num_particles; i++) {
            int x = rand() % PARTICLE_AREA_SIDE_LENGTHS + PARTICLE_AREA_TOP_LEFT.x;
            int y = rand() % PARTICLE_AREA_SIDE_LENGTHS + PARTICLE_AREA_TOP_LEFT.y;
            particles.emplace_back(x, y, idx_to_color[i % num_colors]);
        }
    }

    void removeParticles(int num_particles) {
        for (int i{0}; i < num_particles; i++) {
            particles.pop_back();
        }
    }

    float getForce(float d, float a) {
        if (d < BETA) {
            return d / BETA - 1;
        }
        if (d < 1) {
            return a * (1 - std::abs(2 * d - BETA - 1) / (1 - BETA));
        }
        return 0;
    }

    void buildPartition() {
        //DIMENSIONS 1320 X 1080
        int x_partitions = 1320 / static_cast<int>(D_MAX);
        int y_partitions = 1080 / static_cast<int>(D_MAX);
        float cell_width = 1320.0f / x_partitions;
        float cell_height = 1080.0f / y_partitions;

        if (D_MAX_updated) {
            partition.resize(x_partitions);
            for (auto& col : partition) {
                col.resize(y_partitions);
                for (auto& cell : col) {
                    cell.reserve(100);
                }
            }
            D_MAX_updated = false;
        }
        for (auto& column : partition) {
            for (auto& cell : column) {
                cell.clear();  // reuse memory instead of reallocating
            }
        }

        for (auto& p : particles) {
            int x = static_cast<int>((p.pos.x - 600.0f) / cell_width);
            int y = static_cast<int>(p.pos.y / cell_height);
            if (x >= 0 && x < x_partitions && y >= 0 && y < y_partitions) {
                partition[x][y].emplace_back(&p);
            }
        }
    }

    sf::Vector2f getAcceleration(int x, int y, Particle *p1) {
        sf::Vector2f accel_accumulator = {0,0};
        int x_partitions = 1320 / static_cast<int>(D_MAX);
        int y_partitions = 1080 / static_cast<int>(D_MAX);

        for (int xi{x - 1}; xi <= x + 1; ++xi) {
            if (xi < 0 || xi >= x_partitions) {
                continue;
            }
            for (int yi{y - 1}; yi <= y + 1; ++yi) {
                if (yi < 0 || yi >= y_partitions) {
                    continue;
                }
                for (auto p2 : partition[xi][yi]) {
                    if (p1->pos == p2->pos) {
                        continue;
                    }
                    const sf::Vector2f d_vec = p2->pos - p1->pos;
                    const float d = d_vec.length();
                    accel_accumulator += D_MAX * d_vec / d * getForce(d * D_MAX_INV, attraction_modifier.attraction_matrix[p1->c_idx][p2->c_idx]);
                }
            }
        }
        return accel_accumulator;
    }

    void updateParticleState(int x, int y, Particle *p) {
        p->vel = DAMPING_FACTOR * p->vel + getAcceleration(x, y, p) * DT;
        p->pos += p->vel * DT;

        if (p->pos.x < 840) {
            p->pos.x = 840;
            p->vel.x *= -1;
        }
        if (p->pos.x > 1920) {
            p->pos.x = 1920;
            p->vel.x *= -1;
        }
        if (p->pos.y < 0) {
            p->pos.y = 0;
            p->vel.y *= -1;
        }
        if (p->pos.y > 1080) {
            p->pos.y = 1080;
            p->vel.y *= -1;
        }
    }

    void tick() {
        DAMPING_FACTOR = std::pow(0.5f, DT / DT_HALF);
        D_MAX_INV = 1.0f / D_MAX;
        int x_partitions = 1320 / static_cast<int>(D_MAX);
        int y_partitions = 1080 / static_cast<int>(D_MAX);
        for (int x{0}; x < x_partitions; ++x) {
            for (int y{0}; y < y_partitions; ++y) {
                for (auto &p : partition[x][y]) {
                    updateParticleState(x, y, p);
                }
            }
        }
    }

    void setNumColors(int num) {
        num_colors = num;
        for (int i{0}; i < num_colors; i++) {
            for (int j{0}; j < num_colors; j++) {
                float box_size = 300.0f / (1.0f + static_cast<float>(num));
                sf::Vector2f temp_tl = attraction_modifier.tl + sf::Vector2f((i + 1) * box_size, (j + 1) * box_size);
                attraction_modifier.button_matrix[i][j] = Button(temp_tl, {box_size, box_size});
            }
        }
    }

    void evenlyDiststributeParticleColors() {
        int i{0};
        for (Particle &p : particles) {
            p.c_idx = i % num_colors;
            p.c = idx_to_color[p.c_idx];
            i++;
        }
    }

    void randomlyDistributeParticleColors() {
        int i;
        for (Particle &p : particles) {
            i = std::rand() % num_colors;
            p.c_idx = i;
            p.c = idx_to_color[i];
        }
    }

    int getFPS(sf::Clock &clock) {
        int fps = static_cast<int>(round(1.0f / clock.restart().asSeconds()));
        return fps;
    }

    int getParticleCount() {
        return particles.size();
    }

    std::array<float, 7> getColorDistribution() {
        std::array<float, 7> color_count = {0};
        int num_particles = getParticleCount();

        for (auto &p : particles) {
            color_count[p.c_idx]++;
        }
        for (int i{0}; i < 7; ++i) {
            color_count[i] = color_count[i] / num_particles;
        }
        return color_count;
    }
};


