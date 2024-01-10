#pragma once

#include <array>

// Describe a 'Particle' class that represents one of the N-bodies in the simulation
class Particle {
    
    public:
        // Constructors
        Particle(const std::array<float, 3>* initial_position, const std::array<float, 3>* initial_velocity, float mass);

        // Define member functions
        void update (const std::array<float, 3>* force, const float timestep);

        // Instantiation of the physical members
        std::array<float, 3> position;
        std::array<float, 3> velocity;
        float mass;
};
