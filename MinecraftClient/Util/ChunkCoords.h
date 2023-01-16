#pragma once
#include "World/Generic/Chunk.h"

template <typename T>
struct ChunkCoords
{
    int X;
    int Y;
    int Z;
    ChunkCoords(int x, int y, int z);
    ChunkCoords();

    constexpr bool operator==(const ChunkCoords& other) const
    {
        return X == other.X && Y == other.Y && Z == other.Z;
    }
};

template <typename T>
struct ChunkComparator
{
    std::size_t operator()(const ChunkCoords<T>& other) const
    {
        const size_t x = other.X >= 0 ? static_cast<unsigned long long>(2 * other.X) : static_cast<unsigned long long>(-2 * other.X - 1);
        const size_t y = other.Y >= 0 ? static_cast<unsigned long long>(2 * other.Y) : static_cast<unsigned long long>(-2 * other.Y - 1);
        const size_t z = other.Z >= 0 ? static_cast<unsigned long long>(2 * other.Z) : static_cast<unsigned long long>(-2 * other.Z - 1);
        const size_t xyPair = x >= y ? x * x + x + y : y * y + x;
        return xyPair >= z ? xyPair * xyPair + xyPair + z : z * z + xyPair;
    }
};

template <typename T>
ChunkCoords<T>::ChunkCoords(const int x, const int y, const int z) : X(x / Chunk<T>::CHUNK_WIDTH), Y(y / Chunk<T>::CHUNK_HEIGHT), Z(z / Chunk<T>::CHUNK_DEPTH)
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

template <typename T>
ChunkCoords<T>::ChunkCoords() : ChunkCoords(0, 0, 0)
{
}
