#define _USE_MATH_DEFINES

#include <iostream>
#include <array>
#include <cmath>

#include "../include/particle.h"
#include "../include/body.h"

// DENSITY constant for the assumed density of the planets
float DENSITY = 4E3;

// Constructor definition
Body::Body(const std::array<double, 3>* initial_position, const std::array<double, 3>* initial_velocity, double mass, float radius)
    : Particle(initial_position, initial_velocity, mass), radius(radius == 0 ? pow(3 * mass / (4 * M_PI * DENSITY), 1/3) : radius) {};
