#include <iostream>
#include <array>

#include "../include/particle.h"

// Constructor definition
Particle::Particle(const std::array<float, 3>* initial_position, const std::array<float, 3>* initial_velocity, float mass)
    : position(*initial_position), velocity(*initial_velocity), mass(mass), radius(-1) {};

// Update the particle
void Particle::update(const std::array<float, 3>* force, float timestep) {

    float a_i;  // A value used to get the accelerations at the ith direction
    for (int i = 0; i < 3; i++) {
        a_i = (*force)[i] / mass;  // Get acceleration
        position[i] += (velocity[i] * timestep) + 0.5 * (a_i * (timestep * timestep));  // Update position with: x(t) = x[t-1] + v*t + v*t^2
        velocity[i] += (a_i * timestep);  // Update velocity
    }
}
