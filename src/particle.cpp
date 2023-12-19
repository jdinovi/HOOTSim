#include <iostream>

#include "../include/particle.h"


// Constructor definition
Particle::Particle(const std::array<double, 3>& initialPosition, const std::array<double, 3>& initialVelocity, double mass) 
    : position(initialPosition), velocity(initialVelocity), mass(mass) {};

// Member function definitions
void Particle::update_position() const {
    // Undefined for now
};

void Particle::update_velocity() const {
    // Undefined for now
};
