#pragma once

#include <array>

// Describe a 'Particle' class that represents one of the N-bodies in the simulation
class Particle {
    
    public:
        // Constructors
        Particle(const std::array<double, 3>& initial_position, const std::array<double, 3>& initial_velocity, float mass);

        // Define member functions
        void update_position(const float timestep);
        void update_velocity(const std::array<double, 3>& force, float timestep);

        // Instantiation of the physical members
        std::array<double, 3> position;
        std::array<double, 3> velocity;
        double mass;
};
