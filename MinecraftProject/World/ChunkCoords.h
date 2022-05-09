//
// Created by amit on 4/21/2022.
//

#ifndef MINECRAFTPROJECT_CHUNKCOORDS_H
#define MINECRAFTPROJECT_CHUNKCOORDS_H
#include <functional>

struct ChunkCoords {
private:
    int X;
    int Y;
    int Z;
public:
    [[nodiscard]] int getX() const;
    [[nodiscard]] int getY() const;
    [[nodiscard]] int getZ() const;
    ChunkCoords(int x, int y, int z);
    ChunkCoords();
    constexpr bool operator==(const ChunkCoords& other) const {
        return X == other.X && Y == other.Y && Z == other.Z;
    }
};

struct ChunkComparator {
    std::size_t operator()(const ChunkCoords& other) const {
        size_t x = other.getX() >= 0 ? static_cast<long long>(2) * other.getX() : static_cast<long long>(-2) * other.getX() - 1;
        size_t y = other.getY() >= 0 ? static_cast<long long>(2) * other.getY() : static_cast<long long>(-2) * other.getY() - 1;
        size_t z = other.getZ() >= 0 ? static_cast<long long>(2) * other.getZ() : static_cast<long long>(-2) * other.getZ() - 1;
        size_t xyPair = x >= y ? x * x + x + y : y * y + x;
        return xyPair >= z ? xyPair * xyPair + xyPair + z : z * z + xyPair;
    }
};
#endif //MINECRAFTPROJECT_CHUNKCOORDS_H
