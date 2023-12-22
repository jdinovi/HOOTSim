#include <iostream>
#include <array>
#include <vector>

#include "../include/particle.h"
#include "../include/environment.h"

int main() {

    // Parameters for simulation
    float timestep = 0.5;

    // Initialize particle specs
    double mass = 1E9;
    std::array<double, 3> initial_position1 = {0, 0, 0};
    std::array<double, 3> initial_position2 = {4, 0, 0};
    std::array<double, 3> initial_velocity1 = {0, 0, 0};
    std::array<double, 3> initial_velocity2 = {0, 0, 0};

    
    // Define the particles
    auto particle1Ptr = std::make_shared<Particle>(&initial_position1, &initial_velocity1, mass);
    auto particle2Ptr = std::make_shared<Particle>(&initial_position2, &initial_velocity2, mass);
    std::vector<std::shared_ptr<Particle>> particles = {particle1Ptr, particle2Ptr};
    
    // Initialize an environment
    GravitationalEnvironment env1(particles, true);

    // Take a step
    env1.simulate(3, timestep);

    // std::cout << particle1.position[0] << "\n";
    // std::cout << particle2.position[0];

    return 0;

};



    // for (int i=0; i<5; i++) {
    //     // Update the particle
    //     particle1.update(&force, timestep);
    // }

    // // Define the force
    // std::array<double, 3> force = {3, 0, 0};
