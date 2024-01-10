#include <array>
#include <vector>
#include <random>
#include <sstream>
#include <iostream>
#include <math.h>

#include "../include/doctest.h" 
#include "../include/particle.h"
#include "../include/environment.h"


//////////// SETUP TESTABLE INSTANCES ////////////
// Parameters
float _G = 6.6743e-11;

// Data path
const char* repoPath = std::getenv("HOOTSIM_PATH");
std::string dataPath = repoPath == nullptr ? "./data" : std::string(repoPath )+ "/data";
std::string configPath = repoPath == nullptr ? "./config" : std::string(repoPath )+ "/config";

// Initialize particle specs
float mass = 1E10;
std::array<float, 3> initial_position1 = {0, 0, 0};
std::array<float, 3> initial_position2 = {4, 0, 0};
std::array<float, 3> initial_velocity1 = {0, 0, 0};
std::array<float, 3> initial_velocity2 = {0, 0, 0};

// Define the particles
auto particle1Ptr = std::make_shared<Particle>(&initial_position1, &initial_velocity1, mass);
auto particle2Ptr = std::make_shared<Particle>(&initial_position2, &initial_velocity2, mass);
std::vector<std::shared_ptr<Particle>> particles = {particle1Ptr, particle2Ptr};

// Initialize a particle environment
GravitationalEnvironment<Particle> env1(particles, true);
GravitationalEnvironment<Particle> env2(particles, true, "funPrefix");



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
    CHECK(env1.envOctree.objPtrs.size() == 0);
    CHECK(env1.envOctree.internal == true);
}

// TEST_CASE("Error Handling for Load Config") {
//     // Test error handling when loading an invalid configuration file
//     CHECK_THROWS(loadConfig("invalid_config.yaml"));
// }

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
    std::vector<std::array<float, 3>> forces = env1.getForces(0.2);

    // Expected force magnitude (will be opposite and equal)
    float fMagExpected = _G * (mass*mass) / 16;

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
    float duration = 2;
    float timestep = 1;

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

    // Check that it resets
    CHECK(env1.time == 0);
}


////////// CONFIG FILE TESTS //////////
TEST_CASE("Load Config File") {

    // Get the default map
    std::map<std::string, std::map<std::string, std::string>> defaultConfig = loadConfig("default.yaml");
    
    // Check some values
    CHECK(defaultConfig["vy"]["dist"] == "normal");
    CHECK(defaultConfig["vy"]["mu"] == "-2");
    CHECK(defaultConfig["vy"]["sigma"] == "4");
    CHECK(defaultConfig["global"]["nParticles"] == "1000");
}

void checkDefaultEnv(GravitationalEnvironment<Particle>& env) {
    // Check that it all looks good... that default vals are:
// mass:
//   dist: constant
//   val: 100000000
// x:
//   dist: uniform
//   min: 0
//   max: 4
// y:
//   dist: uniform
//   min: -2
//   max: 4
// z:
//   dist: uniform
//   min: 1
//   max: 10
// vx:
//   dist: uniform
//   min: 0
//   max: 4
// vy:
//   dist: uniform
//   min: -2
//   max: 4
// vz:
//   dist: uniform
//   min: -10
//   max: 10
    bool massCheck = true;
    bool xCheck = true;
    bool yCheck = true;
    bool zCheck = true;
    for (auto pPtr : env.particlePtrs) {
        massCheck &= pPtr->mass == 100000000;
        xCheck &= (pPtr->position[0] <= 4 && pPtr->position[0] >= 0);
        yCheck &= (pPtr->position[1] <= 4 && pPtr->position[1] >= -2);
        zCheck &= (pPtr->position[2] <= 10 && pPtr->position[2] >= 1);
    }
    CHECK(env.nParticles == 1000);
    CHECK(xCheck);
    CHECK(yCheck);
    CHECK(zCheck);
    CHECK(massCheck);
}
TEST_CASE("Load Particles From Config") {

    // Get a defualt environment class
    GravitationalEnvironment<Particle> defaultEnv("default.yaml", true);
    GravitationalEnvironment<Particle> defaultEnv2("default.yaml", true, "prefixyprefix");
    
    // Check 'em
    checkDefaultEnv(defaultEnv);
    checkDefaultEnv(defaultEnv2);
    CHECK(defaultEnv2.logFileName == dataPath + "/prefixyprefix0.csv");
}


////////// CONFIG FILE TESTS //////////
TEST_CASE("Load Config File") {

    // Get the default map
    std::map<std::string, std::map<std::string, std::string>> defaultConfig = loadConfig("default.yaml");
    
    // Check some values
    CHECK(defaultConfig["vy"]["dist"] == "normal");
    CHECK(defaultConfig["vy"]["mu"] == "-2");
    CHECK(defaultConfig["vy"]["sigma"] == "4");
    CHECK(defaultConfig["global"]["nParticles"] == "1000");
}


TEST_CASE("Barnes-Hut Algorithm Force Calculation") {

    float bodyMass = 1;
    std::array<float, 3> body_pos1 = {9, 9, 9};
    std::array<float, 3> body_pos2 = {-9, -9, -9};
    std::array<float, 3> body_pos3 = {-7, -7, -7};
    std::array<float, 3> body_velo1 = {0, 0, 0};
    std::array<float, 3> body_velo2 = {0, 0, 0};
    std::array<float, 3> body_velo3 = {0, 0, 0};

    // Define the particles
    auto body1Ptr = std::make_shared<Body>(&body_pos1, &body_velo1, bodyMass);
    auto body2Ptr = std::make_shared<Body>(&body_pos2, &body_velo2, bodyMass);
    auto body3Ptr = std::make_shared<Body>(&body_pos3, &body_velo3, bodyMass);
    std::vector<std::shared_ptr<Particle>> bodies = {body1Ptr, body2Ptr, body3Ptr};

    GravitationalEnvironment<Particle> env3(bodies, true, "run", "Barnes-Hut");
    std::vector<std::array<float, 3UL>> forces = env3.getForces(0.1);
    
    // First Body Force
    CHECK(forces[0][0] - (-1 * _G * 2 / (sqrt(3) * pow(17, 2))) < 1E-7);
    CHECK(forces[0][1] - (-1 * _G * 2 / (sqrt(3) * pow(17, 2))) < 1E-7);
    CHECK(forces[0][2] - (-1 * _G * 2 / (sqrt(3) * pow(17, 2))) < 1E-7);

    // Second Body Force
    CHECK(forces[1][0] - (_G / (sqrt(3) * (pow(243, -0.5) + pow(12, -0.5)))) < 1E-7);
    CHECK(forces[1][1] - (_G / (sqrt(3) * (pow(243, -0.5) + pow(12, -0.5)))) < 1E-7);
    CHECK(forces[1][2] - (_G / (sqrt(3) * (pow(243, -0.5) + pow(12, -0.5)))) < 1E-7);

    // Third Body Force
    CHECK(forces[2][0] - (-1 * _G * -7 / 48.) < 1E-7);
    CHECK(forces[2][1] - (-1 * _G * -7 / 48.) < 1E-7);
    CHECK(forces[2][2] - (-1 * _G * -7 / 48.) < 1E-7);

}