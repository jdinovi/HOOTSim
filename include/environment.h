#include <iostream>
#include <vector>
#include <array>
#include "./particle.h"


class GravitationalEnvironment{
    
    public:
        // Constructors
        GravitationalEnvironment(const std::vector<Particle*>& particlePtrs);

        // Define member functions
        std::vector<std::array<double, 3>> getForces(const double timestep);
        void updateAll(const std::vector<std::array<double, 3>>* forces, const double timestep);
        void step(const double timestep);
        void reset();

        // Instantiation of the physical members
        std::vector<Particle*> particlePtrs;
        double time;
        int n_particles = particlePtrs.size();

};
