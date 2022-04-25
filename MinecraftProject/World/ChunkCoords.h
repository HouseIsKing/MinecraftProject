//
// Created by amit on 4/21/2022.
//

#ifndef MINECRAFTPROJECT_CHUNKCOORDS_H
#define MINECRAFTPROJECT_CHUNKCOORDS_H
#include <functional>

struct ChunkCoords {
private:
    int x;
    int y;
    int z;
public:
    static constexpr unsigned short chunkSizeX = 8;
    static constexpr unsigned short chunkSizeY = 8;
    static constexpr unsigned short chunkSizeZ = 8;
    [[nodiscard]] int getX() const;
    [[nodiscard]] int getY() const;
    [[nodiscard]] int getZ() const;
    ChunkCoords(int x, int y, int z);
    ChunkCoords();
    constexpr bool operator==(const ChunkCoords& other) const {
        return x == other.x && y == other.y && z == other.z;
    }
};

struct ChunkComparator {
public:
    std::size_t operator()(const ChunkCoords& other) const {
        return ((other.getZ() / ChunkCoords::chunkSizeX << 20) & 0x7FF00000) | (other.getZ() < 0 ? 0x80000000 : 0) | ((other.getY() / ChunkCoords::chunkSizeY << 10) & 0x7FC00) | (other.getY() < 0 ? 0x80000 : 0) | (other.getX() / ChunkCoords::chunkSizeZ & 0x1FF) | (other.getX() < 0 ? 0x200 : 0);
    }
};
#endif //MINECRAFTPROJECT_CHUNKCOORDS_H
