#include <iostream>
#include <array>
#include <vector>
#include <math.h>
#include "../include/environment.h"

double G = 6.6743e-11;


// Constructor definition
GravitationalEnvironment::GravitationalEnvironment(const std::vector<Particle>& particles)
    : particles(particles), time(0) {};


// Get the forces in the environment
std::vector<std::array<double, 3>> GravitationalEnvironment::getForces(const double timestep) {
    // A vector to hold the forces on each particle
    std::vector<std::array<double, 3>> forces(n_particles);

    // Iterate through and find each source contribution
    double prop_to_force;  // Gmm
    double r_dep; // rhat // r^2
    for (int i = 0; i < n_particles; i++) {
        for (int j = i; j < n_particles; j++) {

            // Only calculate Gmm
            prop_to_force = G * particles[i].mass * particles[j].mass;

            for (int k = 0; k < 3; k++) {
                // r-dependence
                r_dep = (particles[i].position[k] - particles[j].position[k]) / pow(particles[i].position[k] - particles[j].position[k], 3);

                // Update forces (opposite and equal)
                forces[i][k] = prop_to_force * r_dep;
                forces[j][k] = -1 * prop_to_force * r_dep;
            }
        }
    }

    return forces;
}


// Update each particle in the environment
void GravitationalEnvironment::updateAll(const std::vector<std::array<double, 3>>& forces, const double timestep) {
    for (int i = 0; i < n_particles; i++){
        particles[i].update(forces[i], timestep);
    }
}


// Take a step
void GravitationalEnvironment::step(const double timestep) {

    // Get the forces and upate everything
    std::vector<std::array<double, 3>> forces = getForces(timestep);
    updateAll(forces, timestep);

    // Update time
    time += timestep;
}


// Reset the environment
void GravitationalEnvironment::reset() {

    time = 0;

}