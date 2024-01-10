#pragma once
#include <vector>
#include <random>

// Assuming you have the random device and generator declared somewhere
extern std::random_device rd;
extern std::mt19937 GENERATOR;

// Distribution template used to sample from random distributions
template <typename Distribution>
std::vector<float> sampleFromDistribution(size_t n, Distribution& distribution) {
    // Generate samples
    std::vector<float> samples(n);
    for (size_t i = 0; i < n; ++i) {
        samples[i] = distribution(GENERATOR);
    }
    return samples;
}

// Dummy executable to work with testing framework
int dummyExecutable();
