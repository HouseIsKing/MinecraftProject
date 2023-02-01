#pragma once
#include "Util/CustomFileManager.h"

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
    bool operator<(const ChunkCoords& chunkCoords) const;
    ChunkCoords(int x, int y, int z);
    ChunkCoords();

    constexpr bool operator==(const ChunkCoords& other) const
    {
        return X == other.X && Y == other.Y && Z == other.Z;
    }

    friend CustomFileManager& operator<<(CustomFileManager& file, const ChunkCoords& coords);
    friend CustomFileManager& operator>>(CustomFileManager& file, ChunkCoords& coords);
};
