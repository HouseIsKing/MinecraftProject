//
// Created by amit on 4/21/2022.
//

#include "ChunkCoords.h"
#include "Chunk.h"

int ChunkCoords::getX() const {
    return x;
}

int ChunkCoords::getY() const {
    return y;
}

int ChunkCoords::getZ() const {
    return z;
}

ChunkCoords::ChunkCoords(const int x, const int y, const int z) : x(x/Chunk::CHUNK_WIDTH), y(y/Chunk::CHUNK_HEIGHT), z(z/Chunk::CHUNK_DEPTH) 
{
    if (x < 0)
    {
        this->x -= 1;
    }
    if (y < 0)
    {
        this->y -= 1;
    }
    if (z < 0)
    {
        this->z -= 1;
    }
}

ChunkCoords::ChunkCoords() : ChunkCoords(0,0,0){}
