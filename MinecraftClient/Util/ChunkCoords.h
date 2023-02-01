#pragma once
#include "EngineDefaults.h"

struct ChunkCoords
{
    int X;
    int Y;
    int Z;
    ChunkCoords(int x, int y, int z);
    ChunkCoords();
    bool operator<(const ChunkCoords& chunkCoords) const;

    constexpr bool operator==(const ChunkCoords& other) const
    {
        return X == other.X && Y == other.Y && Z == other.Z;
    }
};

inline ChunkCoords::ChunkCoords(const int x, const int y, const int z) : X(x / EngineDefaults::CHUNK_WIDTH), Y(y / EngineDefaults::CHUNK_HEIGHT), Z(z / EngineDefaults::CHUNK_DEPTH)
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

inline ChunkCoords::ChunkCoords() : ChunkCoords(0, 0, 0)
{
}

inline bool ChunkCoords::operator<(const ChunkCoords& chunkCoords) const
{
    if (X < chunkCoords.X)
    {
        return true;
    }
    if (X > chunkCoords.X)
    {
        return false;
    }
    if (Y < chunkCoords.Y)
    {
        return true;
    }
    if (Y > chunkCoords.Y)
    {
        return false;
    }
    return Z < chunkCoords.Z;
}

