#pragma once

#include <vector>
#include <memory>

#include "body.h"

template <typename T>
class Octree {
    public:

        // Octree constructor
        Octree(std::array<float, 2>& xCoords, std::array<float, 2>& yCoords, std::array<float, 2>& zCoords, bool internal);

        // Member functions
        void clear();
        void updateCoords(std::array<float, 2>& newXCoords, std::array<float, 2>& newYCoords, std::array<float, 2>& newZCoords);
        void insert(std::shared_ptr<T> objPtr);
        void build(std::vector<std::shared_ptr<T>>& objPtrs);

        // Members
        std::vector<std::shared_ptr<T>> objPtrs;
        std::array<float, 3> centerOfMass;
        float* totalMass;
        bool internal;

        // Dimensions of the current octant
        std::array<float, 2> xCoords;
        std::array<float, 2> yCoords;
        std::array<float, 2> zCoords;

        // Octree children --> 0-7 based on 2D convention in postive z, and then 2D convention in negative z, observing from above        
        std::shared_ptr<Octree<T>> child0;
        std::shared_ptr<Octree<T>> child1;
        std::shared_ptr<Octree<T>> child2;
        std::shared_ptr<Octree<T>> child3;
        std::shared_ptr<Octree<T>> child4;
        std::shared_ptr<Octree<T>> child5;
        std::shared_ptr<Octree<T>> child6;
        std::shared_ptr<Octree<T>> child7;


};
