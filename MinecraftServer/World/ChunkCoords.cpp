#include "ChunkCoords.h"
#include "Util/EngineDefaults.h"

int ChunkCoords::GetX() const
{
    return X;
}

int ChunkCoords::GetY() const
{
    return Y;
}

int ChunkCoords::GetZ() const
{
    return Z;
}

bool ChunkCoords::operator<(const ChunkCoords& chunkCoords) const
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

ChunkCoords::ChunkCoords(const int x, const int y, const int z) : X(x / EngineDefaults::CHUNK_WIDTH), Y(y / EngineDefaults::CHUNK_HEIGHT), Z(z / EngineDefaults::CHUNK_DEPTH)
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

ChunkCoords::ChunkCoords() : ChunkCoords(0, 0, 0)
{
}

CustomFileManager& operator<<(CustomFileManager& file, const ChunkCoords& coords)
{
    file << coords.X << coords.Y << coords.Z;
    return file;
}

CustomFileManager& operator>>(CustomFileManager& file, ChunkCoords& coords)
{
    file >> coords.X >> coords.Y >> coords.Z;
    return file;
}
