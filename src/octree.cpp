#include "./../include/octree.h"
#include <array>
#include <memory>
#include <iostream>

template <typename T>
Octree<T>::Octree(std::array<float, 2>& xCoords, std::array<float, 2>& yCoords, std::array<float, 2>& zCoords, bool internal)
    : totalMass(new float(0)), internal(internal), xCoords(xCoords), yCoords(yCoords), zCoords(zCoords) {};

// Recursively set every child to null in the tree, but preserving the tree
template <typename T>
void Octree<T>::clear() {    
    // Clear the children
    child0.reset();
    child1.reset();
    child2.reset();
    child3.reset();
    child4.reset();
    child5.reset();
    child6.reset();
    child7.reset();

    // Clear the members
    objPtrs.clear();
    *totalMass = 0.0;
}

template <typename T>
void Octree<T>::updateCoords(std::array<float, 2>& newXCoords, std::array<float, 2>& newYCoords, std::array<float, 2>& newZCoords) {
    // Update the coordinates
    xCoords = newXCoords;
    yCoords = newYCoords;
    zCoords = newZCoords;
}

template <typename T>
void Octree<T>::insert(std::shared_ptr<T> objPtr) {

    // Append objPtr to the vector of objects
    objPtrs.push_back(objPtr);

    // Instantiate the center of mass if it doesn't exist; update it otherwise
    float newTotalMass = *totalMass + objPtr->mass;
    if (*totalMass == 0) {
        std::copy(std::begin(objPtr->position), std::end(objPtr->position), std::begin(centerOfMass));
    } else {
        for (int i = 0; i < 3; i++) {
            centerOfMass[i] = (((centerOfMass[i] * (*totalMass))) + ((objPtr->position[i]) * (objPtr->mass))) / (newTotalMass);
        }
    }

    // Update the total mass
    *totalMass = newTotalMass;

    // Deal with recursive insertion based on internal vs external nodes //

    // If current node is internal, then need to recursively insert just the current obj
    if (internal) {

        // Midpoints of coordinates
        float mX = (xCoords[0] + xCoords[1]) / 2.;
        float mY = (yCoords[0] + yCoords[1]) / 2.;
        float mZ = (zCoords[0] + zCoords[1]) / 2.;

        // Initialize flags for octant location
        bool xFlag = objPtr->position[0] > mX;
        bool yFlag = objPtr->position[1] > mY; 
        bool zFlag = objPtr->position[2] > mZ;
    
        std::array<float, 2> xCoordsNew;
        std::array<float, 2> yCoordsNew;
        std::array<float, 2> zCoordsNew;

        // Get the new X coordinates
        if (xFlag) {
            xCoordsNew[0] = mX;
            xCoordsNew[1] = xCoords[1];
        } else {
            xCoordsNew[0] = xCoords[0];
            xCoordsNew[1] = mX;
        }

        // Get the new Y coordinates
        if (yFlag) {
            yCoordsNew[0] = mY;
            yCoordsNew[1] = yCoords[1];
        } else {
            yCoordsNew[0] = yCoords[0];
            yCoordsNew[1] = mY;
        }

        // Get the new Z coordinates
        if (zFlag) {
            zCoordsNew[0] = mZ;
            zCoordsNew[1] = zCoords[1];
        } else {
            zCoordsNew[0] = zCoords[0];
            zCoordsNew[1] = mZ;
        }

        // Insert the new octree into the correct child with if statements (both location, and if child exists)
        if (xFlag & yFlag & zFlag) {
            if (child0 == nullptr) {
                // Instantiate a new octree with the calculated coordinates
                auto newOctreePtr = std::make_unique<Octree<T>>(xCoordsNew, yCoordsNew, zCoordsNew, false);
                child0 = std::move(newOctreePtr);
            }
            child0->insert(objPtr);
        } else if (!xFlag & yFlag & zFlag) {
            if (child1 == nullptr) {
                // Instantiate a new octree with the calculated coordinates
                auto newOctreePtr = std::make_unique<Octree<T>>(xCoordsNew, yCoordsNew, zCoordsNew, false);
                child1 = std::move(newOctreePtr);
            }
            child1->insert(objPtr);
        } else if (!xFlag & !yFlag & zFlag) {
            if (child2 == nullptr) {
                // Instantiate a new octree with the calculated coordinates
                auto newOctreePtr = std::make_unique<Octree<T>>(xCoordsNew, yCoordsNew, zCoordsNew, false);
                child2 = std::move(newOctreePtr);
            }
            child2->insert(objPtr);
        } else if (xFlag & !yFlag & zFlag) {
            if (child3 == nullptr) {
                // Instantiate a new octree with the calculated coordinates
                auto newOctreePtr = std::make_unique<Octree<T>>(xCoordsNew, yCoordsNew, zCoordsNew, false);
                child3 = std::move(newOctreePtr);
            }
            child3->insert(objPtr);
        } else if (xFlag & yFlag & !zFlag) {
            if (child4 == nullptr) {
                // Instantiate a new octree with the calculated coordinates
                auto newOctreePtr = std::make_unique<Octree<T>>(xCoordsNew, yCoordsNew, zCoordsNew, false);
                child4 = std::move(newOctreePtr);
            }
            child4->insert(objPtr);
        } else if (!xFlag & yFlag & !zFlag) {
            if (child5 == nullptr) {
                // Instantiate a new octree with the calculated coordinates
                auto newOctreePtr = std::make_unique<Octree<T>>(xCoordsNew, yCoordsNew, zCoordsNew, false);
                child5 = std::move(newOctreePtr);
            }
            child5->insert(objPtr);
        } else if (!xFlag & !yFlag & !zFlag) {
            if (child6 == nullptr) {
                // Instantiate a new octree with the calculated coordinates
                auto newOctreePtr = std::make_unique<Octree<T>>(xCoordsNew, yCoordsNew, zCoordsNew, false);
                child6 = std::move(newOctreePtr);
            }
            child6->insert(objPtr);
        } else if (xFlag & !yFlag & !zFlag) {
            if (child7 == nullptr) {
                // Instantiate a new octree with the calculated coordinates
                auto newOctreePtr = std::make_unique<Octree<T>>(xCoordsNew, yCoordsNew, zCoordsNew, false);
                child7 = std::move(newOctreePtr);
            }
            child7->insert(objPtr);
        }

    } else if ((!internal) & (objPtrs.size() > 1)) {
        // This current node should now be internal
        internal = true;
        for (std::shared_ptr<T> currObjPtr : objPtrs) {

            // Midpoints of coordinates
            float mX = (xCoords[0] + xCoords[1]) / 2.;
            float mY = (yCoords[0] + yCoords[1]) / 2.;
            float mZ = (zCoords[0] + zCoords[1]) / 2.;

            // Initialize flags for octant location
            bool xFlag = currObjPtr->position[0] > mX;
            bool yFlag = currObjPtr->position[1] > mY; 
            bool zFlag = currObjPtr->position[2] > mZ;
        
            std::array<float, 2> xCoordsNew;
            std::array<float, 2> yCoordsNew;
            std::array<float, 2> zCoordsNew;

            // Get the new X coordinates
            if (xFlag) {
                xCoordsNew[0] = mX;
                xCoordsNew[1] = xCoords[1];
            } else {
                xCoordsNew[0] = xCoords[0];
                xCoordsNew[1] = mX;
            }

            // Get the new Y coordinates
            if (yFlag) {
                yCoordsNew[0] = mY;
                yCoordsNew[1] = yCoords[1];
            } else {
                yCoordsNew[0] = yCoords[0];
                yCoordsNew[1] = mY;
            }

            // Get the new Z coordinates
            if (zFlag) {
                zCoordsNew[0] = mZ;
                zCoordsNew[1] = zCoords[1];
            } else {
                zCoordsNew[0] = zCoords[0];
                zCoordsNew[1] = mZ;
            }

            // Insert the current octree into the correct child with if statements (both location, and if child exists)
            if (xFlag & yFlag & zFlag) {
                if (child0 == nullptr) {
                    // Instantiate a new octree with the calculated coordinates
                    auto newOctreePtr = std::make_unique<Octree<T>>(xCoordsNew, yCoordsNew, zCoordsNew, false);
                    child0 = std::move(newOctreePtr);
                }
                child0->insert(currObjPtr);
            } else if (!xFlag & yFlag & zFlag) {
                if (child1 == nullptr) {
                    // Instantiate a new octree with the calculated coordinates
                    auto newOctreePtr = std::make_unique<Octree<T>>(xCoordsNew, yCoordsNew, zCoordsNew, false);
                    child1 = std::move(newOctreePtr);
                }
                child1->insert(currObjPtr);
            } else if (!xFlag & !yFlag & zFlag) {
                if (child2 == nullptr) {
                    // Instantiate a new octree with the calculated coordinates
                    auto newOctreePtr = std::make_unique<Octree<T>>(xCoordsNew, yCoordsNew, zCoordsNew, false);
                    child2 = std::move(newOctreePtr);
                }
                child2->insert(currObjPtr);
            } else if (xFlag & !yFlag & zFlag) {
                if (child3 == nullptr) {
                    // Instantiate a new octree with the calculated coordinates
                    auto newOctreePtr = std::make_unique<Octree<T>>(xCoordsNew, yCoordsNew, zCoordsNew, false);
                    child3 = std::move(newOctreePtr);
                }
                child3->insert(currObjPtr);
            } else if (xFlag & yFlag & !zFlag) {
                if (child4 == nullptr) {
                    // Instantiate a new octree with the calculated coordinates
                    auto newOctreePtr = std::make_unique<Octree<T>>(xCoordsNew, yCoordsNew, zCoordsNew, false);
                    child4 = std::move(newOctreePtr);
                }
                child4->insert(currObjPtr);
            } else if (!xFlag & yFlag & !zFlag) {
                if (child5 == nullptr) {
                    // Instantiate a new octree with the calculated coordinates
                    auto newOctreePtr = std::make_unique<Octree<T>>(xCoordsNew, yCoordsNew, zCoordsNew, false);
                    child5 = std::move(newOctreePtr);
                }
                child5->insert(currObjPtr);
            } else if (!xFlag & !yFlag & !zFlag) {
                if (child6 == nullptr) {
                    // Instantiate a new octree with the calculated coordinates
                    auto newOctreePtr = std::make_unique<Octree<T>>(xCoordsNew, yCoordsNew, zCoordsNew, false);
                    child6 = std::move(newOctreePtr);
                }
                child6->insert(currObjPtr);
            } else if (xFlag & !yFlag & !zFlag) {
                if (child7 == nullptr) {
                    // Instantiate a new octree with the calculated coordinates
                    auto newOctreePtr = std::make_unique<Octree<T>>(xCoordsNew, yCoordsNew, zCoordsNew, false);
                    child7 = std::move(newOctreePtr);
                }
                child7->insert(currObjPtr);
            }
        }
    }
}

// Build the octree
template <typename T>
void Octree<T>::build(std::vector<std::shared_ptr<T>>& objPtrs) {
    for (std::shared_ptr<T> currObjPtr : objPtrs) {
        this->insert(currObjPtr);
    }
}

template class Octree<Particle>;
template class Octree<Body>;
