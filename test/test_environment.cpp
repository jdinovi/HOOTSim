#include <array>
#include <vector>
#include <sstream>
#include <iostream>

#include "../include/doctest.h" 
#include "../include/particle.h"
#include "../include/environment.h"


//////////// SETUP TESTABLE INSTANCES ////////////
// Parameters
double _G = 6.6743e-11;

// Data path
const char* repoPath = std::getenv("HOOTSIM_PATH");
std::string dataPath = std::string(repoPath) + "/data";

// Initialize particle specs
double mass = 1E10;
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
GravitationalEnvironment env2(particles, true, "funPrefix");





//////////// TEST CASES ////////////
TEST_CASE("Environment Initialization") {

    // Assertions
    CHECK(env1.time == 0);
    CHECK(env1.particlePtrs.size() == 2);
    CHECK(env1.nParticles == 2);
    CHECK(env1.logFileName == dataPath + "/run0.csv");
    CHECK(env2.time == 0);
    CHECK(env2.particlePtrs.size() == 2);
    CHECK(env2.nParticles == 2);
    CHECK(env2.logFileName == dataPath + "/funPrefix0.csv");
}

void test_normalCase() {
    std::vector<std::string> filenames = {"run0.csv", "run1.csv", "run10.csv"};
    CHECK(getLargestLabelNumber(filenames, "run") == 10);
}

void test_noMatchForPrefix() {
    std::vector<std::string> filenames = {"test0.csv", "test1.csv"};
    CHECK(getLargestLabelNumber(filenames, "run") == -1);
}

void test_emptyVector() {
    std::vector<std::string> filenames = {};
    CHECK(getLargestLabelNumber(filenames, "run") == -1);
}

void test_filenameWithPrefixButNoCsv() {
    std::vector<std::string> filenames = {"run0", "run1"};
    CHECK(getLargestLabelNumber(filenames, "run") == -1);
}

void test_prefixFollowedByCsvDirectly() {
    std::vector<std::string> filenames = {"run.csv", "run1.csv"};
    CHECK(getLargestLabelNumber(filenames, "run") == 1);
}
// Add these test cases to the TEST_CASE that already tests getLargestLabelNumber
TEST_CASE("Using getLargestLabelNumber") {
    // Existing tests...
    
    // New test cases
    test_normalCase();
    test_noMatchForPrefix();
    test_emptyVector();
    test_filenameWithPrefixButNoCsv();
    test_prefixFollowedByCsvDirectly();
}

TEST_CASE("Test getLogHeader") {

    // Assertions
    CHECK(env1.getLogHeader() == "Time,mass0,x0,y0,z0,vx0,vy0,vz0,mass1,x1,y1,z1,vx1,vy1,vz1\n");
}

TEST_CASE("Environment getForces") {

    // Get forces
    std::vector<std::array<double, 3>> forces = env1.getForces(0.2);

    // Expected force magnitude (will be opposite and equal)
    double fMagExpected = _G * (mass*mass) / 16;

    // Assertions
    CHECK(forces[0][0] - fMagExpected < 1E-7);
    CHECK(forces[0][1] == 0);
    CHECK(forces[0][2] == 0);
    CHECK(forces[1][0] + fMagExpected < 1E-7);
    CHECK(forces[1][1] == 0);
    CHECK(forces[1][2] == 0);
}

TEST_CASE("Environment Single Step") {

    // Parameters for simulation
    float timestep = 1;

    // Take a step
    env1.step(timestep);

    // Assertions
    CHECK(abs(particle1Ptr->position[0] - 0.0208572) < 0.0001);
    CHECK(abs(particle1Ptr->position[1]) < 1E-8);
    CHECK(env1.time == 1);

}

TEST_CASE("Environment simulate") {

    // Set params
    double duration = 2;
    double timestep = 1;

    // Simulate
    env2.simulate(duration, timestep);

    // Assertions
    CHECK(particle1Ptr->position[0] > 2 * 0.0208572);
    CHECK(particle1Ptr->position[1] < 10 - (2 * 0.0208572));
    CHECK(env2.time == 2);

    // Check the line where we fail to open the file
    // Redirect std::cerr to a stringstream
    std::stringstream buffer;
    std::streambuf* prevCerrStreamBuf = std::cerr.rdbuf();
    std::cerr.rdbuf(buffer.rdbuf());
    
    // Simulate
    env2.logFileName = "/invalid/path/to/logfile.log";
    env2.simulate(duration, timestep);

    // Restore original std::cerr buffer
    std::cerr.rdbuf(prevCerrStreamBuf);

    // Check if the error message is as expected
    std::string output = buffer.str();
    CHECK(output.find("Failed to open the file:") != std::string::npos);
}

TEST_CASE("Reset Environment") {

    // Reset environment
    env1.reset();

    CHECK(env1.time == 0);
}
