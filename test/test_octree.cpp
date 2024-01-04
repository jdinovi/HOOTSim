#define _USE_MATH_DEFINES

#include <array>
#include <cmath>
#include <memory>
#include <iostream>

#include "../include/doctest.h" 
#include "../include/body.h"
#include "../include/particle.h"
#include "../include/octree.h"


// Build an Octree for Testing //
std::array<float, 2> xCoords = {-10., 10.};
std::array<float, 2> yCoords = {-10., 10.};
std::array<float, 2> zCoords = {-10., 10.};

Octree<Body> myOctree(xCoords, yCoords, zCoords, true);

// Build three bodies for insertion
float oct_mass = 1;
float radius = 1E5;

std::array<float, 3> oct_init_pos1 = {5, 5, 6};
std::array<float, 3> oct_init_velo1 = {0, 0, 0};

std::array<float, 3> oct_init_pos2 = {-2.5, -2.5, 6};
std::array<float, 3> oct_init_velo2 = {0, 0, 0};

std::array<float, 3> oct_init_pos3 = {-7.5, -7.5, 6};
std::array<float, 3> oct_init_velo3 = {0, 0, 0};

std::shared_ptr<Body> body1 = std::make_shared<Body>(&oct_init_pos1, &oct_init_velo1, oct_mass, radius);
std::shared_ptr<Body> body2 = std::make_shared<Body>(&oct_init_pos2, &oct_init_velo2, oct_mass, radius);
std::shared_ptr<Body> body3 = std::make_shared<Body>(&oct_init_pos3, &oct_init_velo3, oct_mass, radius);

std::vector<std::shared_ptr<Body>> bodyPtrs = {body1, body2, body3};


TEST_CASE("Octree Initialization") {

    CHECK(myOctree.xCoords[0] == -10.);
    CHECK(myOctree.xCoords[1] == 10.);
    CHECK(myOctree.yCoords[0] == -10.);
    CHECK(myOctree.yCoords[1] == 10.);
    CHECK(myOctree.zCoords[0] == -10.);
    CHECK(myOctree.zCoords[1] == 10.);

    CHECK(*(myOctree.totalMass) == 0);
    CHECK(myOctree.internal == true); 

    CHECK(myOctree.child1 == nullptr);
    CHECK(myOctree.child2 == nullptr);
    CHECK(myOctree.child3 == nullptr);
    CHECK(myOctree.child4 == nullptr);
    CHECK(myOctree.child5 == nullptr);
    CHECK(myOctree.child6 == nullptr);
    CHECK(myOctree.child7 == nullptr);

}

TEST_CASE("Octree Body Insertion") {
    
    // First body insertion and test cases
    myOctree.insert(body1);
    CHECK(myOctree.internal == true);
    CHECK(myOctree.objPtrs.size() == 1);
    CHECK(*(myOctree.totalMass) == oct_mass);

    CHECK(myOctree.child0->centerOfMass[0] == 5.);
    CHECK(myOctree.child0->objPtrs.size() == 1);
    CHECK(myOctree.child0->internal == false);
    CHECK(myOctree.child0->xCoords[0] == 0);
    CHECK(myOctree.child0->xCoords[1] == 10);
    CHECK(myOctree.child0->yCoords[0] == 0);
    CHECK(myOctree.child0->yCoords[1] == 10);
    CHECK(myOctree.child0->zCoords[0] == 0);
    CHECK(myOctree.child0->zCoords[1] == 10);

    // Second body insertion and test cases
    myOctree.insert(body2);
    CHECK(myOctree.internal == true);
    CHECK(myOctree.objPtrs.size() == 2);
    CHECK(*(myOctree.totalMass) == 2);

    CHECK(myOctree.child2->centerOfMass[0] == -2.5);
    CHECK(myOctree.child2->objPtrs.size() == 1);
    CHECK(myOctree.child2->internal == false);
    CHECK(myOctree.child2->xCoords[0] == -10);
    CHECK(myOctree.child2->xCoords[1] == 0);
    CHECK(myOctree.child2->yCoords[0] == -10);
    CHECK(myOctree.child2->yCoords[1] == 0);
    CHECK(myOctree.child2->zCoords[0] == 0);
    CHECK(myOctree.child2->zCoords[1] == 10);

    // Third body insertion and test cases
    myOctree.insert(body3);
    CHECK(myOctree.internal == true);
    CHECK(myOctree.objPtrs.size() == 3);
    CHECK(*(myOctree.totalMass) == 3);

    CHECK(myOctree.child2->objPtrs.size() == 2);
    CHECK(myOctree.child2->internal == true);
    CHECK(myOctree.child2->centerOfMass[0] == -5);
    CHECK(myOctree.child2->centerOfMass[1] == -5);
    CHECK(myOctree.child2->centerOfMass[2] == 6);
    CHECK(*(myOctree.child2->totalMass) == 2);
    CHECK(myOctree.child2->xCoords[0] == -10);
    CHECK(myOctree.child2->xCoords[1] == 0);
    CHECK(myOctree.child2->yCoords[0] == -10);
    CHECK(myOctree.child2->yCoords[1] == 0);
    CHECK(myOctree.child2->zCoords[0] == 0);
    CHECK(myOctree.child2->zCoords[1] == 10);

    CHECK(myOctree.child2->child0->objPtrs.size() == 1);
    CHECK(myOctree.child2->child0->internal == false);
    CHECK(*(myOctree.child2->child0->totalMass) == 1);
    CHECK(myOctree.child2->child0->xCoords[0] == -5);
    CHECK(myOctree.child2->child0->xCoords[1] == 0);
    CHECK(myOctree.child2->child0->yCoords[0] == -5);
    CHECK(myOctree.child2->child0->yCoords[1] == 0);
    CHECK(myOctree.child2->child0->zCoords[0] == 5);
    CHECK(myOctree.child2->child0->zCoords[1] == 10);
    CHECK(myOctree.child2->child0->centerOfMass[0] == -2.5);
    CHECK(myOctree.child2->child0->centerOfMass[1] == -2.5);
    CHECK(myOctree.child2->child0->centerOfMass[2] == 6);

    CHECK(myOctree.child2->child2->objPtrs.size() == 1);
    CHECK(myOctree.child2->child2->internal == false);
    CHECK(*(myOctree.child2->child2->totalMass) == 1);
    CHECK(myOctree.child2->child2->xCoords[0] == -10);
    CHECK(myOctree.child2->child2->xCoords[1] == -5);
    CHECK(myOctree.child2->child2->yCoords[0] == -10);
    CHECK(myOctree.child2->child2->yCoords[1] == -5);
    CHECK(myOctree.child2->child2->zCoords[0] == 5);
    CHECK(myOctree.child2->child2->zCoords[1] == 10);
    CHECK(myOctree.child2->child2->centerOfMass[0] == -7.5);
    CHECK(myOctree.child2->child2->centerOfMass[1] == -7.5);
    CHECK(myOctree.child2->child2->centerOfMass[2] == 6);
}

TEST_CASE("Clearing Octree") {
    myOctree.clear();
    CHECK(myOctree.child0 == nullptr);
    CHECK(myOctree.child1 == nullptr);
    CHECK(myOctree.child2 == nullptr);
    CHECK(myOctree.child3 == nullptr);
    CHECK(myOctree.child4 == nullptr);
    CHECK(myOctree.child5 == nullptr);
    CHECK(myOctree.child6 == nullptr);
    CHECK(myOctree.child7 == nullptr);

    CHECK(myOctree.objPtrs.size() == 0);
    CHECK(*(myOctree.totalMass) == 0);
}


TEST_CASE("Octree Build Function") {
    myOctree.build(bodyPtrs);

    // All bodies inserted and test cases
    CHECK(myOctree.internal == true);
    CHECK(myOctree.objPtrs.size() == 3);
    CHECK(*(myOctree.totalMass) == 3);

    CHECK(myOctree.child2->objPtrs.size() == 2);
    CHECK(myOctree.child2->internal == true);
    CHECK(myOctree.child2->centerOfMass[0] == -5);
    CHECK(myOctree.child2->centerOfMass[1] == -5);
    CHECK(myOctree.child2->centerOfMass[2] == 6);
    CHECK(*(myOctree.child2->totalMass) == 2);
    CHECK(myOctree.child2->xCoords[0] == -10);
    CHECK(myOctree.child2->xCoords[1] == 0);
    CHECK(myOctree.child2->yCoords[0] == -10);
    CHECK(myOctree.child2->yCoords[1] == 0);
    CHECK(myOctree.child2->zCoords[0] == 0);
    CHECK(myOctree.child2->zCoords[1] == 10);

    CHECK(myOctree.child2->child0->objPtrs.size() == 1);
    CHECK(myOctree.child2->child0->internal == false);
    CHECK(*(myOctree.child2->child0->totalMass) == 1);
    CHECK(myOctree.child2->child0->xCoords[0] == -5);
    CHECK(myOctree.child2->child0->xCoords[1] == 0);
    CHECK(myOctree.child2->child0->yCoords[0] == -5);
    CHECK(myOctree.child2->child0->yCoords[1] == 0);
    CHECK(myOctree.child2->child0->zCoords[0] == 5);
    CHECK(myOctree.child2->child0->zCoords[1] == 10);
    CHECK(myOctree.child2->child0->centerOfMass[0] == -2.5);
    CHECK(myOctree.child2->child0->centerOfMass[1] == -2.5);
    CHECK(myOctree.child2->child0->centerOfMass[2] == 6);

    CHECK(myOctree.child2->child2->objPtrs.size() == 1);
    CHECK(myOctree.child2->child2->internal == false);
    CHECK(*(myOctree.child2->child2->totalMass) == 1);
    CHECK(myOctree.child2->child2->xCoords[0] == -10);
    CHECK(myOctree.child2->child2->xCoords[1] == -5);
    CHECK(myOctree.child2->child2->yCoords[0] == -10);
    CHECK(myOctree.child2->child2->yCoords[1] == -5);
    CHECK(myOctree.child2->child2->zCoords[0] == 5);
    CHECK(myOctree.child2->child2->zCoords[1] == 10);
    CHECK(myOctree.child2->child2->centerOfMass[0] == -7.5);
    CHECK(myOctree.child2->child2->centerOfMass[1] == -7.5);
    CHECK(myOctree.child2->child2->centerOfMass[2] == 6);

    CHECK(abs(myOctree.centerOfMass[0] - (-5. / 3.)) < 0.00001);
    CHECK(abs(myOctree.centerOfMass[1] - (-5. / 3.)) < 0.00001);
    CHECK(myOctree.centerOfMass[2] == 6);
}

TEST_CASE("Update the Coordinates of Root Octree") {
    std::array<float, 2> newXCoords = {-15., 5.};
    std::array<float, 2> newYCoords = {-15., 5.};
    std::array<float, 2> newZCoords = {-15., 5.};

    myOctree.clear();
    myOctree.updateCoords(newXCoords, newYCoords, newZCoords);

    CHECK(myOctree.xCoords[0] == -15);
    CHECK(myOctree.xCoords[1] == 5);

    CHECK(myOctree.yCoords[0] == -15);
    CHECK(myOctree.yCoords[1] == 5);

    CHECK(myOctree.zCoords[0] == -15);
    CHECK(myOctree.zCoords[1] == 5);
}