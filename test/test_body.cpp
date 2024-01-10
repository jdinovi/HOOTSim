#define _USE_MATH_DEFINES

#include <array>
#include <cmath>

#include "../include/doctest.h" 
#include "../include/body.h"

TEST_CASE("Body Initialization - WITH Radius") {

    // Initialize particle specs
    float mass = 1;
    std::array<float, 3> initial_position1 = {0, 0, 0};
    std::array<float, 3> initial_velocity1 = {0, 0, 0};
    float radius = 1E5;

    // Define the particle
    Body body1(&initial_position1, &initial_velocity1, mass, radius);

    CHECK(body1.position[0] == 0);
    CHECK(body1.position[1] == 0);
    CHECK(body1.position[2] == 0);
    CHECK(body1.mass == 1);
    CHECK(body1.radius == 1E5);
}

TEST_CASE("Body Initialization - WITHOUT Radius") {

    // Initialize particle specs
    float mass = 1;
    std::array<float, 3> initial_position1 = {0, 0, 0};
    std::array<float, 3> initial_velocity1 = {0, 0, 0};

    // Define the particle
    Body body1(&initial_position1, &initial_velocity1, mass);

    CHECK(body1.position[0] == 0);
    CHECK(body1.position[1] == 0);
    CHECK(body1.position[2] == 0);
    CHECK(body1.mass == 1);
    CHECK((body1.radius - pow(3 * mass / (4 * M_PI * 4E3), 1./3.)) < 0.000001);
}


TEST_CASE("Body Update") {

    // Parameters for a timestep
    float timestep = 1;

    // Initialize particle specs
    float mass = 1;
    float radius = 1E5;
    std::array<float, 3> initial_position1 = {0, 0, 0};
    std::array<float, 3> initial_velocity1 = {0, 0, 0};

    // Define the force
    std::array<float, 3> force = {3, 0, 0};

    // Define the particle
    Body body1(&initial_position1, &initial_velocity1, mass, radius);

    // Update particle
    body1.update(&force, timestep);
    
    // Check the update works
    CHECK(body1.position[0] == 1.5);
}