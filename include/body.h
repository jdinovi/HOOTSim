#pragma once

#include <vector>
#include <array>

#include "./particle.h"

// Describe a 'Body' class that represents one of the N-bodies in the simulation, inherited from 'Particle'
class Body : public Particle {
    
    public:
        
        // Child class constructor
        explicit Body(const std::array<float, 3>* initial_position, const std::array<float, 3>* initial_velocity, float mass, float radius=0);

        // New physical member
        float radius;
};
