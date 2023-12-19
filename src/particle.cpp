#include <iostream>
#include <array>

#include "../include/particle.h"

// Constructor definition
Particle::Particle(const std::array<double, 3>& initial_position, const std::array<double, 3>& initial_velocity, float mass)
    : position(initial_position), velocity(initial_velocity), mass(mass) {};

// Update the position of the particle based on the current velocity
void Particle::update_position(const float timestep) {

    for (int i=0; i<3; ++i) {
        position[i] += (velocity[i] * timestep);
    };

};

// Update the velocity of the particle based on the force experienced by the particle
void Particle::update_velocity(const std::array<double, 3>& force, float timestep) {

    int a;
    for (int i=0; i<3; i++) {
        a = force[i] / mass;
        velocity[i] += (a * timestep);
    };
};
