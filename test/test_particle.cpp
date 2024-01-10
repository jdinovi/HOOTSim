#include <array>

#include "../include/doctest.h" 
#include "../include/particle.h"

TEST_CASE("Particle Initialization") {

    // Initialize particle specs
    float mass = 1;
    std::array<float, 3> initial_position1 = {0, 0, 0};
    std::array<float, 3> initial_velocity1 = {0, 0, 0};

    // Define the particle
    Particle particle1(&initial_position1, &initial_velocity1, mass);

    CHECK(particle1.position[0] == 0);
    CHECK(particle1.position[1] == 0);
    CHECK(particle1.position[2] == 0);
    CHECK(particle1.mass == 1);
}

TEST_CASE("Particle Update") {

    // Parameters for a timestep
    float timestep = 1;

    // Initialize particle specs
    float mass = 1;
    std::array<float, 3> initial_position1 = {0, 0, 0};
    std::array<float, 3> initial_velocity1 = {0, 0, 0};

    // Define the force
    std::array<float, 3> force = {3, 0, 0};

    // Define the particle
    Particle particle1(&initial_position1, &initial_velocity1, mass);

    // Update particle
    particle1.update(&force, timestep);
    
    // Check the update works
    CHECK(particle1.position[0] == 1.5);
}