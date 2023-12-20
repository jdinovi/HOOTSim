#pragma once

#include <array>

// Describe a 'Particle' class that represents one of the N-bodies in the simulation
class Particle {
    
    public:
        // Constructors
        Particle(const std::array<double, 3>* initial_position, const std::array<double, 3>* initial_velocity, double mass);

        // Define member functions
        void update (const std::array<double, 3>* force, const double timestep);

        // Instantiation of the physical members
        std::array<double, 3> position;
        std::array<double, 3> velocity;
        double mass;
};
