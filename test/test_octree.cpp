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

    CHECK(myOctree.totalMass == 0);
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
    CHECK(myOctree.totalMass == oct_mass);

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
    CHECK(myOctree.totalMass == 2);

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
    CHECK(myOctree.totalMass == 3);

    CHECK(myOctree.child2->objPtrs.size() == 2);
    CHECK(myOctree.child2->internal == true);
    CHECK(myOctree.child2->centerOfMass[0] == -5);
    CHECK(myOctree.child2->centerOfMass[1] == -5);
    CHECK(myOctree.child2->centerOfMass[2] == 6);
    CHECK(myOctree.child2->totalMass == 2);
    CHECK(myOctree.child2->xCoords[0] == -10);
    CHECK(myOctree.child2->xCoords[1] == 0);
    CHECK(myOctree.child2->yCoords[0] == -10);
    CHECK(myOctree.child2->yCoords[1] == 0);
    CHECK(myOctree.child2->zCoords[0] == 0);
    CHECK(myOctree.child2->zCoords[1] == 10);

    CHECK(myOctree.child2->child0->objPtrs.size() == 1);
    CHECK(myOctree.child2->child0->internal == false);
    CHECK(myOctree.child2->child0->totalMass == 1);
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
    CHECK(myOctree.child2->child2->totalMass == 1);
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
    myOctree.clearOctree();
    CHECK(myOctree.child0 == nullptr);
    CHECK(myOctree.child1 == nullptr);
    CHECK(myOctree.child2 == nullptr);
    CHECK(myOctree.child3 == nullptr);
    CHECK(myOctree.child4 == nullptr);
    CHECK(myOctree.child5 == nullptr);
    CHECK(myOctree.child6 == nullptr);
    CHECK(myOctree.child7 == nullptr);

    CHECK(myOctree.objPtrs.size() == 0);
    CHECK(myOctree.totalMass == 0);
}


TEST_CASE("Octree Build Function") {
    myOctree.build(bodyPtrs);

    // All bodies inserted and test cases
    CHECK(myOctree.internal == true);
    CHECK(myOctree.objPtrs.size() == 3);
    CHECK(myOctree.totalMass == 3);

    CHECK(myOctree.child2->objPtrs.size() == 2);
    CHECK(myOctree.child2->internal == true);
    CHECK(myOctree.child2->centerOfMass[0] == -5);
    CHECK(myOctree.child2->centerOfMass[1] == -5);
    CHECK(myOctree.child2->centerOfMass[2] == 6);
    CHECK(myOctree.child2->totalMass == 2);
    CHECK(myOctree.child2->xCoords[0] == -10);
    CHECK(myOctree.child2->xCoords[1] == 0);
    CHECK(myOctree.child2->yCoords[0] == -10);
    CHECK(myOctree.child2->yCoords[1] == 0);
    CHECK(myOctree.child2->zCoords[0] == 0);
    CHECK(myOctree.child2->zCoords[1] == 10);

    CHECK(myOctree.child2->child0->objPtrs.size() == 1);
    CHECK(myOctree.child2->child0->internal == false);
    CHECK(myOctree.child2->child0->totalMass == 1);
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
    CHECK(myOctree.child2->child2->totalMass == 1);
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

    myOctree.clearOctree();
    myOctree.updateCoords(newXCoords, newYCoords, newZCoords);

    CHECK(myOctree.xCoords[0] == -15);
    CHECK(myOctree.xCoords[1] == 5);

    CHECK(myOctree.yCoords[0] == -15);
    CHECK(myOctree.yCoords[1] == 5);

    CHECK(myOctree.zCoords[0] == -15);
    CHECK(myOctree.zCoords[1] == 5);
}

TEST_CASE("Insert Into Each Child") {

    std::array<float, 3> posChild0 = {9, 9, 9};
    std::array<float, 3> veloChild0 = {0, 0, 0};
    auto bodyChild0 = std::make_shared<Body>(&posChild0, &veloChild0, oct_mass, radius);

    std::array<float, 3> posChild1 = {-9, 9, 9};
    std::array<float, 3> veloChild1 = {0, 0, 0};
    auto bodyChild1 = std::make_shared<Body>(&posChild1, &veloChild1, oct_mass, radius);

    std::array<float, 3> posChild2 = {-9, -9, 9};
    std::array<float, 3> veloChild2 = {0, 0, 0};
    auto bodyChild2 = std::make_shared<Body>(&posChild2, &veloChild2, oct_mass, radius);

    std::array<float, 3> posChild3 = {9, -9, 9};
    std::array<float, 3> veloChild3 = {0, 0, 0};
    auto bodyChild3 = std::make_shared<Body>(&posChild3, &veloChild3, oct_mass, radius);

    std::array<float, 3> posChild4 = {9, 9, -9};
    std::array<float, 3> veloChild4 = {0, 0, 0};
    auto bodyChild4 = std::make_shared<Body>(&posChild4, &veloChild4, oct_mass, radius);

    std::array<float, 3> posChild5 = {-9, 9, -9};
    std::array<float, 3> veloChild5 = {0, 0, 0};
    auto bodyChild5 = std::make_shared<Body>(&posChild5, &veloChild5, oct_mass, radius);

    std::array<float, 3> posChild6 = {-9, -9, -9};
    std::array<float, 3> veloChild6 = {0, 0, 0};
    auto bodyChild6 = std::make_shared<Body>(&posChild6, &veloChild6, oct_mass, radius);

    std::array<float, 3> posChild7 = {9, -9, -9};
    std::array<float, 3> veloChild7 = {0, 0, 0};
    auto bodyChild7 = std::make_shared<Body>(&posChild7, &veloChild7, oct_mass, radius);

    std::vector<std::shared_ptr<Body>> bodyChildPtrs = {bodyChild0, bodyChild1, bodyChild2, bodyChild3, bodyChild4, bodyChild5, bodyChild6, bodyChild7};

    // Clear and build the tree
    myOctree.clearOctree();
    myOctree.build(bodyChildPtrs);

    // Test cases for each child
    // Child0:
    CHECK(myOctree.child0->objPtrs[0]->position[0] == 9);
    CHECK(myOctree.child0->objPtrs[0]->position[1] == 9);
    CHECK(myOctree.child0->objPtrs[0]->position[2] == 9);
    CHECK(myOctree.child0->objPtrs.size() == 1);

    // Child1:
    CHECK(myOctree.child1->objPtrs[0]->position[0] == -9);
    CHECK(myOctree.child1->objPtrs[0]->position[1] == 9);
    CHECK(myOctree.child1->objPtrs[0]->position[2] == 9);
    CHECK(myOctree.child1->objPtrs.size() == 1);

    // Child2:
    CHECK(myOctree.child2->objPtrs[0]->position[0] == -9);
    CHECK(myOctree.child2->objPtrs[0]->position[1] == -9);
    CHECK(myOctree.child2->objPtrs[0]->position[2] == 9);
    CHECK(myOctree.child2->objPtrs.size() == 1);

    // Child3:
    CHECK(myOctree.child3->objPtrs[0]->position[0] == 9);
    CHECK(myOctree.child3->objPtrs[0]->position[1] == -9);
    CHECK(myOctree.child3->objPtrs[0]->position[2] == 9);
    CHECK(myOctree.child3->objPtrs.size() == 1);

    // Child4:
    CHECK(myOctree.child4->objPtrs[0]->position[0] == 9);
    CHECK(myOctree.child4->objPtrs[0]->position[1] == 9);
    CHECK(myOctree.child4->objPtrs[0]->position[2] == -9);
    CHECK(myOctree.child4->objPtrs.size() == 1);

    // Child5:
    CHECK(myOctree.child5->objPtrs[0]->position[0] == -9);
    CHECK(myOctree.child5->objPtrs[0]->position[1] == 9);
    CHECK(myOctree.child5->objPtrs[0]->position[2] == -9);
    CHECK(myOctree.child5->objPtrs.size() == 1);

    // Child6:
    CHECK(myOctree.child6->objPtrs[0]->position[0] == -9);
    CHECK(myOctree.child6->objPtrs[0]->position[1] == -9);
    CHECK(myOctree.child6->objPtrs[0]->position[2] == -9);
    CHECK(myOctree.child6->objPtrs.size() == 1);

    // Child7:
    CHECK(myOctree.child7->objPtrs[0]->position[0] == 9);
    CHECK(myOctree.child7->objPtrs[0]->position[1] == -9);
    CHECK(myOctree.child7->objPtrs[0]->position[2] == -9);
    CHECK(myOctree.child7->objPtrs.size() == 1);


    // Perform the tests that trigger recursive insertion //
    // Rebuild the bodies for this situation
    posChild0 = {9, 9, 9};
    posChild1 = {4, 9, 9};
    posChild2 = {4, 4, 9};
    posChild3 = {9, 4, 9};
    posChild4 = {9, 9, 4};
    posChild5 = {4, 9, 4};
    posChild6 = {4, 4, 4};
    posChild7 = {9, 4, 4};

    bodyChild0 = std::make_shared<Body>(&posChild0, &veloChild0, oct_mass, radius);
    bodyChild1 = std::make_shared<Body>(&posChild1, &veloChild1, oct_mass, radius);
    bodyChild2 = std::make_shared<Body>(&posChild2, &veloChild2, oct_mass, radius);
    bodyChild3 = std::make_shared<Body>(&posChild3, &veloChild3, oct_mass, radius);
    bodyChild4 = std::make_shared<Body>(&posChild4, &veloChild4, oct_mass, radius);
    bodyChild5 = std::make_shared<Body>(&posChild5, &veloChild5, oct_mass, radius);
    bodyChild6 = std::make_shared<Body>(&posChild6, &veloChild6, oct_mass, radius);
    bodyChild7 = std::make_shared<Body>(&posChild7, &veloChild7, oct_mass, radius);

    bodyChildPtrs = {bodyChild0, bodyChild1, bodyChild2, bodyChild3, bodyChild4, bodyChild5, bodyChild6, bodyChild7};
    
    // Build a new octree
    Octree<Body> testOctree(xCoords, yCoords, zCoords, true);
    testOctree.build(bodyChildPtrs);

    // Test cases for each child
    // Initial Child0
    CHECK(testOctree.child0->totalMass == 8);
    CHECK(testOctree.child0->objPtrs.size() == 8);

    // SubChild0:
    CHECK(testOctree.child0->child0->objPtrs[0]->position[0] == 9);
    CHECK(testOctree.child0->child0->objPtrs[0]->position[1] == 9);
    CHECK(testOctree.child0->child0->objPtrs[0]->position[2] == 9);
    CHECK(testOctree.child0->child0->objPtrs.size() == 1);
    
    CHECK(testOctree.child0->child0->xCoords[0] == 5);
    CHECK(testOctree.child0->child0->xCoords[1] == 10);
    CHECK(testOctree.child0->child0->yCoords[0] == 5);
    CHECK(testOctree.child0->child0->yCoords[1] == 10);
    CHECK(testOctree.child0->child0->zCoords[0] == 5);
    CHECK(testOctree.child0->child0->zCoords[1] == 10);

    // SubChild1:
    CHECK(testOctree.child0->child1->objPtrs[0]->position[0] == 4);
    CHECK(testOctree.child0->child1->objPtrs[0]->position[1] == 9);
    CHECK(testOctree.child0->child1->objPtrs[0]->position[2] == 9);
    CHECK(testOctree.child0->child1->objPtrs.size() == 1);

    CHECK(testOctree.child0->child1->xCoords[0] == 0);
    CHECK(testOctree.child0->child1->xCoords[1] == 5);
    CHECK(testOctree.child0->child1->yCoords[0] == 5);
    CHECK(testOctree.child0->child1->yCoords[1] == 10);
    CHECK(testOctree.child0->child1->zCoords[0] == 5);
    CHECK(testOctree.child0->child1->zCoords[1] == 10);

    // SubChild2:
    CHECK(testOctree.child0->child2->objPtrs[0]->position[0] == 4);
    CHECK(testOctree.child0->child2->objPtrs[0]->position[1] == 4);
    CHECK(testOctree.child0->child2->objPtrs[0]->position[2] == 9);
    CHECK(testOctree.child0->child2->objPtrs.size() == 1);

    CHECK(testOctree.child0->child2->xCoords[0] == 0);
    CHECK(testOctree.child0->child2->xCoords[1] == 5);
    CHECK(testOctree.child0->child2->yCoords[0] == 0);
    CHECK(testOctree.child0->child2->yCoords[1] == 5);
    CHECK(testOctree.child0->child2->zCoords[0] == 5);
    CHECK(testOctree.child0->child2->zCoords[1] == 10);

    // SubChild3:
    CHECK(testOctree.child0->child3->objPtrs[0]->position[0] == 9);
    CHECK(testOctree.child0->child3->objPtrs[0]->position[1] == 4);
    CHECK(testOctree.child0->child3->objPtrs[0]->position[2] == 9);
    CHECK(testOctree.child0->child3->objPtrs.size() == 1);

    CHECK(testOctree.child0->child3->xCoords[0] == 5);
    CHECK(testOctree.child0->child3->xCoords[1] == 10);
    CHECK(testOctree.child0->child3->yCoords[0] == 0);
    CHECK(testOctree.child0->child3->yCoords[1] == 5);
    CHECK(testOctree.child0->child3->zCoords[0] == 5);
    CHECK(testOctree.child0->child3->zCoords[1] == 10);

    // SubChild4:
    CHECK(testOctree.child0->child4->objPtrs[0]->position[0] == 9);
    CHECK(testOctree.child0->child4->objPtrs[0]->position[1] == 9);
    CHECK(testOctree.child0->child4->objPtrs[0]->position[2] == 4);
    CHECK(testOctree.child0->child4->objPtrs.size() == 1);

    CHECK(testOctree.child0->child4->xCoords[0] == 5);
    CHECK(testOctree.child0->child4->xCoords[1] == 10);
    CHECK(testOctree.child0->child4->yCoords[0] == 5);
    CHECK(testOctree.child0->child4->yCoords[1] == 10);
    CHECK(testOctree.child0->child4->zCoords[0] == 0);
    CHECK(testOctree.child0->child4->zCoords[1] == 5);

    // SubChild5:
    CHECK(testOctree.child0->child5->objPtrs[0]->position[0] == 4);
    CHECK(testOctree.child0->child5->objPtrs[0]->position[1] == 9);
    CHECK(testOctree.child0->child5->objPtrs[0]->position[2] == 4);
    CHECK(testOctree.child0->child5->objPtrs.size() == 1);

    CHECK(testOctree.child0->child5->xCoords[0] == 0);
    CHECK(testOctree.child0->child5->xCoords[1] == 5);
    CHECK(testOctree.child0->child5->yCoords[0] == 5);
    CHECK(testOctree.child0->child5->yCoords[1] == 10);
    CHECK(testOctree.child0->child5->zCoords[0] == 0);
    CHECK(testOctree.child0->child5->zCoords[1] == 5);

    // SubChild6:
    CHECK(testOctree.child0->child6->objPtrs[0]->position[0] == 4);
    CHECK(testOctree.child0->child6->objPtrs[0]->position[1] == 4);
    CHECK(testOctree.child0->child6->objPtrs[0]->position[2] == 4);
    CHECK(testOctree.child0->child6->objPtrs.size() == 1);

    CHECK(testOctree.child0->child6->xCoords[0] == 0);
    CHECK(testOctree.child0->child6->xCoords[1] == 5);
    CHECK(testOctree.child0->child6->yCoords[0] == 0);
    CHECK(testOctree.child0->child6->yCoords[1] == 5);
    CHECK(testOctree.child0->child6->zCoords[0] == 0);
    CHECK(testOctree.child0->child6->zCoords[1] == 5);

    // SubChild7:
    CHECK(testOctree.child0->child7->objPtrs[0]->position[0] == 9);
    CHECK(testOctree.child0->child7->objPtrs[0]->position[1] == 4);
    CHECK(testOctree.child0->child7->objPtrs[0]->position[2] == 4);
    CHECK(testOctree.child0->child7->objPtrs.size() == 1);

    CHECK(testOctree.child0->child7->xCoords[0] == 5);
    CHECK(testOctree.child0->child7->xCoords[1] == 10);
    CHECK(testOctree.child0->child7->yCoords[0] == 0);
    CHECK(testOctree.child0->child7->yCoords[1] == 5);
    CHECK(testOctree.child0->child7->zCoords[0] == 0);
    CHECK(testOctree.child0->child7->zCoords[1] == 5);

}
