#include <array>
#include <vector>

#include "../include/doctest.h" 
#include "../include/particle.h"
#include "../include/environment.h"

TEST_CASE("Environment Initialization") {

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
    
    // Initialize an environment
    GravitationalEnvironment env1(particles, false);

    // Assertions
    CHECK(env1.time == 0);
    CHECK(env1.particlePtrs.size() == 2);

}

TEST_CASE("Environment Single Step") {

    // Parameters for simulation
    float timestep = 1;

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
    
    // Initialize an environment
    GravitationalEnvironment env1(particles, false);

    // Take a step
    env1.step(timestep);

    // Assertions
    CHECK(std::abs(particle1.position[0] - 0.0208572) < 0.0001);
    CHECK(env1.time == 1);

}




