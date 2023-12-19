#pragma once

#include <array>

// Describe a 'Particle' class that represents one of the N-bodies in the simulation
class Particle {
    
    public:
        // Constructors
        Particle(const std::array<double, 3>& initialPosition, const std::array<double, 3>& initialVelocity, double mass);

        // Define member functions
        void update_position() const;
        void update_velocity() const;

        // Instantiation of the physical members
        std::array<double, 3> position;
        std::array<double, 3> velocity;
        double mass;
};
