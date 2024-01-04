#pragma once
#include <vector>
#include <array>
#include <memory>
#include <functional>

#include "./particle.h"
#include "./octree.h"

template <typename T>
class GravitationalEnvironment{
    
    public:
        // Constructors
        // GravitationalEnvironment(const std::vector<std::shared_ptr<T>>& particlePtrs, const bool log);
        GravitationalEnvironment(const std::vector<std::shared_ptr<T>>& particlePtrs,
                                 const bool log,
                                 std::string logFilePrefix="run",
                                 std::string forceAlgorithm="pair-wise",
                                 std::string interactionType="none",
                                 std::string interactionAlgorithm="pair-wise",
                                 float COR=1.0);

        // Callable members
        std::function<std::vector<std::array<float, 3>>(float)> getForces;   // Either pair-wise or Barnes-Hut force algorithm
        std::function<void()> getInteractions;  // Either pair-wise or spatial partition algorithms
        std::function<void(std::shared_ptr<T> a, std::shared_ptr<T> b, float COR)> interact;  // Either pair-wise or spatial partition algorithms

        // Define member functions for force algorithms
        std::vector<std::array<float, 3>> getForcesPairWise(const float timestep);
        std::vector<std::array<float, 3>> getForcesBarnesHut(const float timestep);

        // Define member functions for getting interactions
        void getInteractionsPairWise();
        void getInteractionsSpatialPartitioned();
        
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
        float COR;
};

// Helper functions
int getLargestLabelNumber(const std::vector<std::string>& filenames, const std::string logFilePrefix);
float getEuclidianDistance(std::array<float, 3> coords1, std::array<float, 3> coords2);
