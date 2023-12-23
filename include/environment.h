#pragma once
#include <vector>
#include <array>
#include <memory> // Include for smart pointers
#include "particle.h"


class GravitationalEnvironment {
public:
    // Constructors
    GravitationalEnvironment(const std::vector<std::shared_ptr<Particle>>& particlePtrs, const bool log);
    GravitationalEnvironment(const std::vector<std::shared_ptr<Particle>>& particlePtrs, const bool log, std::string logFilePrefix);

    // Define member functions
    std::vector<std::array<double, 3>> getForces(const double timestep);
    void updateAll(const std::vector<std::array<double, 3>>& forces, const double timestep);
    void step(const double timestep);
    void simulate(const double duration, const double timestep);
    std::string getStepLog() const;
    std::string getLogHeader() const;
    void reset();

    // Instantiation of the physical members
    std::vector<std::shared_ptr<Particle>> particlePtrs; // Changed to std::shared_ptr
    bool log;
    double time;
    int nParticles; // This should be calculated in the constructor.
    std::string logFileName;

};

// Helper functions
int getLargestLabelNumber(const std::vector<std::string>& filenames, const std::string logFilePrefix);
