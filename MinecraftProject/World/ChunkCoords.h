//
// Created by amit on 4/21/2022.
//

#pragma once
#include <ostream>

#include "../Util/CustomFileManager.h"

struct ChunkCoords
{
private:
    int X;
    int Y;
    int Z;
public:
    [[nodiscard]] int GetX() const;
    [[nodiscard]] int GetY() const;
    [[nodiscard]] int GetZ() const;
    ChunkCoords(int x, int y, int z);
    ChunkCoords();

    constexpr bool operator==(const ChunkCoords& other) const
    {
        return X == other.X && Y == other.Y && Z == other.Z;
    }

    friend CustomFileManager& operator<<(CustomFileManager& file, const ChunkCoords& coords);
    friend CustomFileManager& operator>>(CustomFileManager& file, ChunkCoords& coords);
};

struct ChunkComparator
{
    std::size_t operator()(const ChunkCoords& other) const
    {
        const size_t x = other.GetX() >= 0 ? static_cast<unsigned long long>(2 * other.GetX()) : static_cast<unsigned long long>(-2 * other.GetX() - 1);
        const size_t y = other.GetY() >= 0 ? static_cast<unsigned long long>(2 * other.GetY()) : static_cast<unsigned long long>(-2 * other.GetY() - 1);
        const size_t z = other.GetZ() >= 0 ? static_cast<unsigned long long>(2 * other.GetZ()) : static_cast<unsigned long long>(-2 * other.GetZ() - 1);
        const size_t xyPair = x >= y ? x * x + x + y : y * y + x;
        return xyPair >= z ? xyPair * xyPair + xyPair + z : z * z + xyPair;
    }
};
