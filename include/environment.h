#include <iostream>
#include <vector>
#include <array>

#include "./particle.h"

template<typename T>
class GravitationalEnvironment{
    
    public:
        // Constructors
        GravitationalEnvironment(const std::vector<T*>& particlePtrs, const bool log);

        // Define member functions
        std::vector<std::array<double, 3>> getForces(const double timestep);
        void updateAll(const std::vector<std::array<double, 3>>* forces, const double timestep);
        void step(const double timestep);
        void simulate(const double duration, const double timestep);
        std::string getStepLog();
        std::string getLogHeader();
        void reset();

        // Instantiation of the physical members
        std::vector<T*> particlePtrs;
        bool log;
        double time;
        int n_particles;
        std::string logFileName;
};
