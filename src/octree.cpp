#include "./../include/octree.h"
#include <array>

template <typename T>
Octree<T>::Octree(std::array<float, 2>& xCoords, std::array<float, 2>& yCoords, std::array<float, 2>& zCoords)
    : xCoords(xCoords), yCoords(yCoords), zCoords(zCoords), totalMass(0) {};

// Recursively set every child to null in the tree, but preserving the tree
template <typename T>
void Octree<T>::clear() {    
    
    if (child0 != nullptr) {
        child0.clear();
        child0 = nullptr;
    }

    if (child1 != nullptr) {
        child1.clear();
        child1 = nullptr;
    }

    if (child2 != nullptr) {
        child2.clear();
        child2 = nullptr;
    }

    if (child3 != nullptr) {
        child3.clear();
        child3 = nullptr;
    }

    if (child4 != nullptr) {
        child4.clear();
        child4 = nullptr;
    }

    if (child5 != nullptr) {
        child5.clear();
        child5 = nullptr;
    }

    if (child6 != nullptr) {
        child6.clear();
        child6 = nullptr;
    }

    if (child7 != nullptr) {
        child7.clear();
        child7 = nullptr;
    }
}

template <typename T>
void Octree<T>::insert(std::shared_ptr<T> objPtr) {

    // Append objPtr to the vector of objects
    objPtrs.push_back(objPtr);

    // Instantiate the center of mass if it doesn't exist; update it otherwise
    float newTotalMass = totalMass + objPtr->mass;
    if (totalMass == 0) {
        centerOfMass = objPtr->position;
    } else {
        for (int i = 0; i < 3; i++) {
            centerOfMass[i] = (((centerOfMass[i] * totalMass)) + ((objPtr->position[i]) * (objPtr->mass))) / (newTotalMass);
        }
    }

    // Update the total mass
    totalMass = newTotalMass;

    // Midpoints of coordinates
    float mX = (xCoords[0] + xCoords[1]) / 2.;
    float mY = (yCoords[0] + yCoords[1]) / 2.;
    float mZ = (zCoords[0] + zCoords[1]) / 2.;


    // Check for recursive insertion --> if greater than 1 pointer, then need to recursive insert
    if (objPtrs.size() > 1) {
        
        // Initialize flags for octant location
        bool xFlag = objPtr->position[0] > mX;
        bool yFlag = objPtr->position[1] > mY; 
        bool zFlag = objPtr->position[2] > mZ;

        std::array<float, 2> xCoordsNew;
        std::array<float, 2> yCoordsNew;
        std::array<float, 2> zCoordsNew;


        // Get the X coordinates
        if (xFlag) {
            xCoordsNew[0] = mX;
            xCoordsNew[1] = xCoords[1];
        } else {
            xCoordsNew[0] = xCoords[0];
            xCoordsNew[1] = mX;
        }

        // Get the Y coordinates
        if (yFlag) {
            yCoordsNew[0] = mY;
            yCoordsNew[1] = yCoords[1];
        } else {
            yCoordsNew[0] = yCoords[0];
            yCoordsNew[1] = mY;
        }

        // Get the Z coordinates
        if (zFlag) {
            zCoordsNew[0] = mZ;
            zCoordsNew[1] = zCoords[1];
        } else {
            zCoordsNew[0] = zCoords[0];
            zCoordsNew[1] = mZ;
        }

        // TO DO: instantiate a new octree with the calculated coordinates and the correct child
        



    }
}