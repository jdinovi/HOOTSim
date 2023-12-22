#include <iostream>
#include <array>
#include <vector>

#include "../include/particle.h"
#include "../include/body.h"
#include "../include/environment.h"

int main() {
 

    // // Parameters for simulation
    // float timestep = 0.5;

    // Initialize particle specs
    double mass = 1E10;
    std::array<double, 3> initial_position1 = {0, 0, 0};
    std::array<double, 3> initial_position2 = {4, 0, 0};
    std::array<double, 3> initial_velocity1 = {0, 0, 0};
    std::array<double, 3> initial_velocity2 = {0, 0, 0};

    
    // Define the particles
    Particle particle1(&initial_position1, &initial_velocity1, mass);
    Particle particle2(&initial_position2, &initial_velocity2, mass);  

    std::vector<Particle*> particles = {&particle1, &particle2};

    using ParticleEnvironment = GravitationalEnvironment<Particle>;

    ParticleEnvironment env1(particles, false);

    double valueAt = (env1.particlePtrs[1])->position[0];
    std::cout << valueAt;

    return 0;

};
