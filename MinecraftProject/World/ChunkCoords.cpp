//
// Created by amit on 4/21/2022.
//

#include "ChunkCoords.h"
#include "Chunk.h"

int ChunkCoords::getX() const {
    return X;
}

int ChunkCoords::getY() const {
    return Y;
}

int ChunkCoords::getZ() const {
    return Z;
}

ChunkCoords::ChunkCoords(const int x, const int y, const int z) : X(x/Chunk::CHUNK_WIDTH), Y(y/Chunk::CHUNK_HEIGHT), Z(z/Chunk::CHUNK_DEPTH) 
{
    if (x < 0)
    {
        this->X -= 1;
    }
    if (y < 0)
    {
        this->Y -= 1;
    }
    if (z < 0)
    {
        this->Z -= 1;
    }
}

ChunkCoords::ChunkCoords() : ChunkCoords(0,0,0){}
