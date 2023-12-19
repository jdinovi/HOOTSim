#include <iostream>
#include <vector>
#include "Particle.h"


class GravitationalEnvironment{
    
    public:
        // Constructors
        GravitationalEnvironment(const std::vector<Particle>& particles);

        // Define member functions
        std::vector<std::array<double, 3>> getForces(const double timestep);
        void updateAll(const std::vector<std::array<double, 3>>& forces, const double timestep);
        void step (const double timestep);

        // Instantiation of the physical members
        std::vector<Particle> particles;
        double time;
        int n_particles = particles.size();
};
