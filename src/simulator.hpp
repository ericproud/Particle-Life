#pragma once
#include <cmath>
#include <vector>
#include "particle.hpp"
#include "config.hpp"
#include "attraction_modifier.hpp"

//const int SCREEN_HEIGHT = 1920;
//const int SCREEN_WIDTH = 1080;

struct Simulator {
    std::vector<Particle> particles;
    std::vector<Particle*> partition[28][28];
    AttractionModifier attraction_modifier;
    Button increase_particles_button;
    Button decrease_particles_button;
    int num_colors;
    int matrix_mode;
    //

    Simulator():
        particles(),
        partition(),
        attraction_modifier({1.0f, 61.0f}, 2),
        increase_particles_button({61, 850}, {30.0f, 30.0f}),
        decrease_particles_button({1, 850}, {30.0f, 30.0f}),
        num_colors(2),
        matrix_mode(0)
    {
        insertParticlesEvenDistribution(1000, 2);
        increase_particles_button.button_zone.setFillColor(sf::Color::Green);
        decrease_particles_button.button_zone.setFillColor(sf::Color::Red);
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
        //min x = 1920 -1080 = 840
        //num partitions = 1080 / 40 = 27 but there is zero so actually 28
        for (int i{0}; i < 28; i++) {
            for (int j{0}; j < 28; j++) {
                partition[i][j].clear();
            }
        }
        //printf("H");
        for (auto &p : particles) {
            int x = static_cast<int>(std::floor((p.pos.x - 840) / 40.0f));
            int y = static_cast<int>(std::floor(p.pos.y / 40.0f));
            //printf("intserting in %d, %d \n", x, y);
            partition[x][y].emplace_back(&p);
        }

    }

    sf::Vector2f getAcceleration(int x, int y, Particle *p1) {
        sf::Vector2f accel_accumulator = {0,0};

        for (int xi{x - 1}; xi <= x + 1; xi++) {
            if (xi < 0 || xi >27) {
                continue;
            }
            for (int yi{y - 1}; yi <= y + 1; yi++) {
                if (yi < 0 || yi >27) {
                    continue;
                }
                for (auto p2 : partition[xi][yi]) {
                    if (p1->pos == p2->pos) {
                        continue;
                    }
                    const sf::Vector2f d_vec = p2->pos - p1->pos;
                    const float d = d_vec.length();
                    const float a = attraction_modifier.attraction_matrix[p1->c_idx][p2->c_idx];
                    accel_accumulator += D_MAX * d_vec / d * getForce(d/ D_MAX, a);
                }
            }
        }
        return accel_accumulator;
    }

    void updateParticleState(int x, int y, Particle *p) {
        sf::Vector2f acceleration = getAcceleration(x, y, p);

        p->vel = std::pow(1.0f / 2.0f, DT / DT_HALF) * p->vel + acceleration * DT;

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
        for (int x{0}; x < 28; x++) {
            for (int y{0}; y < 28; y++) {
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

    int getFPS(sf::Clock &clock) {
        int fps = static_cast<int>(round(1.0f / clock.restart().asSeconds()));
        return fps;
    }

    int getParticleCount() {
        return particles.size();
    }
};


