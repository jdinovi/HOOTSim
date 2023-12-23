#include <iostream>
#include <array>
#include <vector>
#include <memory>

#include "../include/particle.h"
#include "../include/body.h"
#include "../include/environment.h"

int main() {
 

    // // Parameters for simulation
    // float timestep = 0.5;

    // Initialize particle specs
    double mass = 1E9;
    std::array<double, 3> initial_position1 = {0, 0, 0};
    std::array<double, 3> initial_position2 = {4, 0, 0};
    std::array<double, 3> initial_velocity1 = {0, 0, 0};
    std::array<double, 3> initial_velocity2 = {0, 0, 0};

    
    // Define the particles
    auto particle1Ptr = std::make_shared<Body>(&initial_position1, &initial_velocity1, mass);
    auto particle2Ptr = std::make_shared<Body>(&initial_position2, &initial_velocity2, mass);
    std::vector<std::shared_ptr<Body>> particles = {particle1Ptr, particle2Ptr};
    
    GravitationalEnvironment<Body> env1(particles, true);

    env1.simulate(3, 0.5);

    return 0;

};
