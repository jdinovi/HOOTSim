#pragma once
#include <vector>
#include <array>
#include <memory>

#include "./particle.h"

template <typename T>
class GravitationalEnvironment{
    
    public:
        // Constructors
        // GravitationalEnvironment(const std::vector<std::shared_ptr<T>>& particlePtrs, const bool log);
        GravitationalEnvironment(const std::vector<std::shared_ptr<T>>& particlePtrs, const bool log, std::string logFilePrefix="run");

        // Define member functions
        std::vector<std::array<double, 3>> getForces(const double timestep);
        void updateAll(const std::vector<std::array<double, 3>>& forces, const double timestep);
        void step(const double timestep);
        void simulate(const double duration, const double timestep);
        std::string getStepLog() const;
        std::string getLogHeader() const;
        void reset();

        // Instantiation of the physical members
        std::vector<std::shared_ptr<T>> particlePtrs; // Changed to std::shared_ptr
        bool log;
        double time;
        int nParticles;
        std::string logFileName;
};

// Helper functions
int getLargestLabelNumber(const std::vector<std::string>& filenames, const std::string logFilePrefix);
