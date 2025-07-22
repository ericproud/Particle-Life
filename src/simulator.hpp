#pragma once
#include <cmath>
#include <vector>
#include "particle.hpp"
#include "config.hpp"
#include "attraction_modifier.hpp"
#include <random>
#include <omp.h>

struct Simulator {
    std::vector<Particle> particles;
    std::vector<std::vector<std::vector<Particle*>>> partition;
    AttractionModifier attraction_modifier;
    ParameterModifier parameter_modifier;
    Button increase_particles_button, decrease_particles_button;
    Button increase_color_count_button, decrease_color_count_button;
    Button reset_matrix_button, randomize_matrix_button, worm_mode_button;
    Button equalize_distribution_button, randomize_distribution_button;
    int num_colors;
    float beta, d_max, d_max_inv, dt, dt_half, damping_factor;
    bool is_worm_mode = false;
    bool d_max_updated = false;
    //

    Simulator():
        //Initializing members
        particles(),
        partition(),
        attraction_modifier({101.0f, 121.0f}, 2),
        parameter_modifier({180, 911}, {180, 971}, {180, 1031}),
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
        beta(0.3f),
        d_max(40),
        d_max_inv(0.02f),
        dt(1.0f / 60.0f),
        dt_half(1.0f/ 120.0f),
        damping_factor(std::pow(0.5f, dt / dt_half))
    {
        //Reserving space for 200,000 particles
        particles.reserve(200000);

        //Reserving space for 20,000 particles per partition
        const int x_partitions = PARTICLE_AREA.x / static_cast<int>(d_max);
        const int y_partitions = PARTICLE_AREA.y / static_cast<int>(d_max);
        partition.resize(x_partitions);
        for (auto& col : partition) {
            col.resize(y_partitions);
            for (auto& cell : col) {
                cell.reserve(10000);
            }
        }

        //Initialize 1000 particles (500 white, 500 red)
        insertParticlesEvenDistribution(1000, 2);

        //Setting some button colors to their proper color
        increase_particles_button.button_zone.setFillColor(sf::Color::Green);
        decrease_particles_button.button_zone.setFillColor(sf::Color::Red);
        increase_color_count_button.button_zone.setFillColor(sf::Color::Green);
        decrease_color_count_button.button_zone.setFillColor(sf::Color::Red);
        worm_mode_button.button_zone.setFillColor(sf::Color::Red);
    }

    float getForce(const float d,const float a) const {
        if (d < beta) {
            return d / beta - 1;
        }
        if (d < 1) {
            return a * (1 - std::abs(2 * d - beta - 1) / (1 - beta));
        }
        return 0;
    }

    void buildPartition() {
        //Particle zone:
        //              Width:   602 <--> 1918
        //              Height:    2 <--> 1078
        //              Dimensions: 1316 X 1076
        const int x_partitions = PARTICLE_AREA.x / static_cast<int>(d_max);
        const int y_partitions = PARTICLE_AREA.y / static_cast<int>(d_max);
        const float cell_width = PARTICLE_AREA.x / x_partitions;
        const float cell_height = PARTICLE_AREA.y / y_partitions;

        //If d_max has been updated, we resize the partition for optimal efficiency
        if (d_max_updated) {
            partition.resize(x_partitions);
            for (auto& col : partition) {
                col.resize(y_partitions);
                for (auto& cell : col) {
                    cell.reserve(10000);
                }
            }
            d_max_updated = false;
        }

        //Clear any remaining particles from the partition
        for (auto& column : partition) {
            for (auto& cell : column) {
                cell.clear();  // reuse memory instead of reallocating
            }
        }

        //Calculate the in
        for (auto& p : particles) {
            const int x = static_cast<int>((p.pos.x - PARTICLE_AREA_TL.x) / cell_width);
            const int y = static_cast<int>((p.pos.y - PARTICLE_AREA_TL.y) / cell_height);
            if (x >= 0 && x < x_partitions && y >= 0 && y < y_partitions) {
                partition[x][y].emplace_back(&p);
            }
        }
    }

    sf::Vector2f getAcceleration(Particle *p1) {
        sf::Vector2f accel_accumulator = {0,0};

        const int x_partitions = PARTICLE_AREA.x / static_cast<int>(d_max);
        const int y_partitions = PARTICLE_AREA.y / static_cast<int>(d_max);
        const float cell_width = PARTICLE_AREA.x / x_partitions;
        const float cell_height = PARTICLE_AREA.y / y_partitions;
        const int x = static_cast<int>((p1->pos.x - PARTICLE_AREA_TL.x) / cell_width);
        const int y = static_cast<int>((p1->pos.y - PARTICLE_AREA_TL.y) / cell_height);

        //Checking the 9 squares around and including particle (also including the wraparound if applicable)
        //    +---+---+---+
        //    |   |   |   |
        //    +---+---+---+
        //    |   | 0 |   |
        //    +---+---+---+
        //    |   |   |   |
        //    +---+---+---+
        for (int xi{x - 2}; xi <= x + 2; ++xi) {
            const int x_wrapped = (xi + x_partitions) % x_partitions;

            if (xi == x - 2 || xi == x + 2) {
                if (x_wrapped == xi) {
                    continue;
                }
            }
            for (int yi{y - 2}; yi <= y + 2; ++yi) {
                const int y_wrapped = (yi + y_partitions) % y_partitions;

                if (yi == y - 2 || yi == y + 2) {
                    if (y_wrapped == yi) {
                        continue;
                    }
                }
                for (auto p2 : partition[x_wrapped][y_wrapped]) {
                    //Avoid d_vec = {0, 0}
                    if (p1->pos == p2->pos) {
                        continue;
                    }

                    //Calculating distance vector and scalar
                    sf::Vector2f d_vec = p2->pos - p1->pos;

                    //Adjusting d_vec to account for wraparound if necessary
                    if (d_vec.x > PARTICLE_AREA_HALF.x) d_vec = sf::Vector2f{d_vec.x - PARTICLE_AREA.x, d_vec.y};
                    else if (d_vec.x < -PARTICLE_AREA_HALF.x) d_vec = sf::Vector2f{d_vec.x + PARTICLE_AREA.x, d_vec.y};

                    if (d_vec.y > PARTICLE_AREA_HALF.y) d_vec = sf::Vector2f{d_vec.x, d_vec.y - PARTICLE_AREA.y};
                    else if (d_vec.y < - PARTICLE_AREA_HALF.y) d_vec = sf::Vector2f{d_vec.x, d_vec.y + PARTICLE_AREA.y};

                    const float d = d_vec.length();

                    //increment accumulated acceleration by the scaled and vectorized force
                    accel_accumulator += d_max * d_vec / d * getForce(d * d_max_inv, attraction_modifier.attraction_matrix[p1->c_idx][p2->c_idx]);
                }
            }
        }
        return accel_accumulator;
    }

    void updateParticleState(Particle *p) {
        p->vel = damping_factor * p->vel + getAcceleration(p) * dt;
        p->pos += p->vel * dt;

        if (p->pos.x < PARTICLE_AREA_TL.x) {
            p->pos.x += PARTICLE_AREA.x;
        }
        if (p->pos.x >= PARTICLE_AREA_BR.x) {
            p->pos.x -= PARTICLE_AREA.x;
        }
        if (p->pos.y < PARTICLE_AREA_TL.y) {
            p->pos.y += PARTICLE_AREA.y;
        }
        if (p->pos.y >= PARTICLE_AREA_BR.y) {
            p->pos.y -= PARTICLE_AREA.y;
        }
    }

    void tick() {
        damping_factor = std::pow(0.5f, dt / dt_half);
        d_max_inv = 1.0f / d_max;
        const int x_partitions = PARTICLE_AREA.x / static_cast<int>(d_max);
        const int y_partitions = PARTICLE_AREA.y / static_cast<int>(d_max);

        //Paralellization directive
        #pragma omp parallel for collapse(2) schedule(dynamic)

        //Update the state of each particle in each subpartition
        for (int x = 0; x < x_partitions; ++x) {
            for (int y = 0; y < y_partitions; ++y) {
                for (auto &p : partition[x][y]) {
                    updateParticleState(p);
                }
            }
        }
    }

    void insertParticlesEvenDistribution(const int num_particles, const int num_colors) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<float> dist_x(PARTICLE_AREA_TL.x, PARTICLE_AREA_BR.x);
        std::uniform_real_distribution<float> dist_y(PARTICLE_AREA_TL.y, PARTICLE_AREA_BR.y);

        for (int i = 0; i < num_particles; ++i) {
            float x = dist_x(gen);
            float y = dist_y(gen);
            particles.emplace_back(x, y, idx_to_color[i % num_colors]);
        }
    }

    void removeParticles(const int num_particles) {
        for (int i{0}; i < num_particles; i++) {
            particles.pop_back();
        }
    }

    void setNumColors(const int num) {
        num_colors = num;
        const float box_size = 400.0f / (1.0f + static_cast<float>(num));

        for (int i{0}; i < num_colors; ++i) {
            for (int j{0}; j < num_colors; ++j) {
                const sf::Vector2f temp_tl = attraction_modifier.tl + sf::Vector2f((i + 1) * box_size, (j + 1) * box_size);
                attraction_modifier.button_matrix[i][j] = Button(temp_tl, {box_size, box_size});
            }
        }
    }

    void evenlyDistributeParticleColors() {
        int i{0};
        for (Particle &p : particles) {
            p.c_idx = i % num_colors;
            p.c = idx_to_color[p.c_idx];
            i++;
        }
    }

    void randomlyDistributeParticleColors() {
        for (Particle &p : particles) {
            int i = rand() % num_colors;
            p.c_idx = i;
            p.c = idx_to_color[i];
        }
    }

    static int getFPS(sf::Clock &clock) {
        int fps = static_cast<int>(round(1.0f / clock.restart().asSeconds()));
        return fps;
    }

    int getParticleCount() const{
        return particles.size();
    }

    std::array<float, 7> getColorDistribution() const {
        std::array<float, 7> color_count = {0};
        const int num_particles = getParticleCount();

        for (auto &p : particles) {
            color_count[p.c_idx]++;
        }
        for (int i{0}; i < 7; ++i) {
            color_count[i] = color_count[i] / num_particles;
        }
        return color_count;
    }

    static float roundToDecimalPlaces(float value, int places) {
        const float scale = std::pow(10.0f, places);
        return std::round(value * scale) / scale;
    }
};


