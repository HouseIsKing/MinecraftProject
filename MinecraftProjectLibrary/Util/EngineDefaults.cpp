#include "EngineDefaults.h"
#include <glm/trigonometric.hpp>

float EngineDefaults::ConvertLightLevelToAmbient(const int lightLevel)
{
    if (lightLevel == 0)
    {
        return 0.6F;
    }
    return 1.0F;
}

uint16_t EngineDefaults::GetChunkLocalIndex(int x, int y, int z)
{
    x %= CHUNK_WIDTH;
    y %= CHUNK_HEIGHT;
    z %= CHUNK_DEPTH;
    if (x < 0)
    {
        x += CHUNK_WIDTH;
    }
    if (y < 0)
    {
        y += CHUNK_HEIGHT;
    }
    if (z < 0)
    {
        z += CHUNK_DEPTH;
    }
    return static_cast<uint16_t>(x + z * CHUNK_WIDTH + y * CHUNK_WIDTH * CHUNK_DEPTH);
}

glm::vec3 TransformStruct::GetForwardVector() const
{
    return {
        glm::cos(glm::radians(Rotation.x) * glm::sin(glm::radians(Rotation.y))), -glm::sin(glm::radians(Rotation.x)), glm::cos(glm::radians(Rotation.x) * glm::cos(glm::radians(Rotation.y)))
    };
}

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
