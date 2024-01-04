#pragma once
#include <vector>
#include <array>
#include <memory>

#include "./particle.h"
#include "./octree.h"

template <typename T>
class GravitationalEnvironment{
    
    public:
        // Constructors
        // GravitationalEnvironment(const std::vector<std::shared_ptr<T>>& particlePtrs, const bool log);
        GravitationalEnvironment(const std::vector<std::shared_ptr<T>>& particlePtrs, const bool log, std::string logFilePrefix="run");

        // Define member functions
        std::vector<std::array<float, 3>> getForces(const float timestep);
        std::vector<std::array<float, 3>> getForcesBarnesHut(const float timestep);
        std::array<float, 3> calculateForceBarnesHut(std::shared_ptr<T> objPtr, std::shared_ptr<Octree<T>> currPtr, std::array<float, 3> netForce, float theta);
        
        void updateAll(const std::vector<std::array<float, 3>>& forces, const float timestep);
        void step(const float timestep);
        void simulate(const float duration, const float timestep);
        std::string getStepLog() const;
        std::string getLogHeader() const;
        void reset();

        // Instantiation of the physical members
        std::vector<std::shared_ptr<T>> particlePtrs; // Changed to std::shared_ptr
        bool log;
        float time;
        int nParticles;
        std::string logFileName;
        Octree<T> envOctree;
};

// Helper functions
int getLargestLabelNumber(const std::vector<std::string>& filenames, const std::string logFilePrefix);
float getEuclidianDistance(std::array<float, 3> coords1, std::array<float, 3> coords2);
