#include <vector>
#include <random>
#include <iostream>

#include "../include/doctest.h"
#include "../include/statistics.h"

TEST_CASE("Constant") {
    // Sample
    std::vector<float> valVec(12, std::stod("6.32"));

    // Check that all the values are equal
    bool allEq = true;
    for (float v : valVec) {
        allEq &= (v - 6.32 < 0.001);
    }
    CHECK(allEq);
    CHECK(valVec.size() == 12);
}

TEST_CASE("Normal") {
    // Sample
    int n = 1000;
    std::normal_distribution<> normalDist(0, 1);
    std::vector<float> valVec = sampleFromDistribution(n, normalDist);

    // Check the mean is near 0
    float mean = 0;
    for (float v : valVec) {
        mean += v;
    }
    mean /= n;
    CHECK(abs(mean) < 0.1);

    // Check std is near 1
    float std = 0;
    for (float v : valVec) {
        std += v*v;
    }
    std = pow(std / n, 0.5);
    CHECK(abs(std - 1) < 0.1);
    CHECK(valVec.size() == n);
}

TEST_CASE("Uniform") {
    // Sample
    int n = 1000;
    std::uniform_real_distribution<> uniformDis(-5, 5);
    std::vector<float> valVec = sampleFromDistribution(n, uniformDis);

    // Check the mean is near 0
    float mean = 0;
    for (float v : valVec) {
        mean += v;
    }
    mean /= n;
    CHECK(abs(mean) < 0.25);

    // Check they're in the sampling range
    bool inRange = true;
    for (float v : valVec) {
        inRange &= (v <= 5 & v >= -5);
    }
    CHECK(inRange);
    CHECK(valVec.size() == n);
}
