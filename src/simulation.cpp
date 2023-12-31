#include <iostream>
#include <array>
#include <vector>
#include <memory>

#include "../include/particle.h"
#include "../include/body.h"
#include "../include/environment.h"

int main() {
    GravitationalEnvironment defaultEnv("default.yaml", true);

    // Simulate
    defaultEnv.simulate(3, 0.5);

    return 0;
};
