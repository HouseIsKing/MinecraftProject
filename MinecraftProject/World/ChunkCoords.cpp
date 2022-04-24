//
// Created by amit on 4/21/2022.
//

#include "ChunkCoords.h"

int ChunkCoords::getX() const {
    return x;
}

int ChunkCoords::getY() const {
    return y;
}

int ChunkCoords::getZ() const {
    return z;
}

ChunkCoords::ChunkCoords(const int x, const int y, const int z) : x(x), y(y), z(z) {}

ChunkCoords::ChunkCoords() : ChunkCoords(0,0,0){}
