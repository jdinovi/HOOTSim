#pragma once
#include <vector>
#include <array>
#include <map>
#include <memory>
#include <functional>

#include "./particle.h"
#include "./octree.h"

template <typename T>
class GravitationalEnvironment{
    
    public:
        // Constructors
        // GravitationalEnvironment(const std::vector<std::shared_ptr<T>>& particlePtrs, const bool log);
        GravitationalEnvironment(const std::vector<std::shared_ptr<T>>& particlePtrs, const bool log, std::string logFilePrefix="run", std::string forceAlgorithm="pair-wise");
        GravitationalEnvironment(const std::string configFileName, const bool log, std::string logFilePrefix = "run");

        // Callable member that we will set to pair-wise or Barnes-Hut force algorithm
        std::function<std::vector<std::array<float, 3>>(float)> getForces;

        // Define member functions for force algorithms
        std::vector<std::array<float, 3>> getForcesPairWise(const float timestep);
        std::vector<std::array<float, 3>> getForcesBarnesHut(const float timestep);
        
        void loadParticlesFromConfig(std::string configFileName);
        std::array<float, 3> calculateForceBarnesHut(std::shared_ptr<T> objPtr, std::shared_ptr<Octree<T>> currPtr, std::array<float, 3> netForce, float theta);
        void updateAll(const std::vector<std::array<float, 3>>& forces, const float timestep);
        void step(const float timestep);
        void simulate(const float duration, const float timestep);
        std::string getStepLog() const;
        std::string getLogHeader() const;
        void reset();
        
        // Instantiation of the physical members
        std::vector<std::shared_ptr<T>> particlePtrs;
        bool log;
        float time;
        int nParticles;
        std::string logFileName;
        Octree<T> envOctree;

    private:
        // Instantiation of the physical members
        std::string logFilePrefix;
};

// Helper functions
int getLargestLabelNumber(const std::vector<std::string>& filenames, const std::string logFilePrefix);
float getEuclidianDistance(std::array<float, 3> coords1, std::array<float, 3> coords2);
std::map<std::string, std::map<std::string, std::string>> loadConfig(const std::string& fileName);
