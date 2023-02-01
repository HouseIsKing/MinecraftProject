#include "EngineDefaults.h"

float EngineDefaults::ConvertLightLevelToAmbient(const int lightLevel)
{
    if (lightLevel == 0)
    {
        return 0.6F;
    }
    return 1.0F;
}

int EngineDefaults::GetChunkLocalIndex(int x, int y, int z)
{
    x %= CHUNK_WIDTH;
    y %= CHUNK_HEIGHT;
    z %= CHUNK_DEPTH;
    return x + z * CHUNK_WIDTH + y * CHUNK_WIDTH * CHUNK_DEPTH;
}

void EngineDefaults::EmplaceDataInVector(std::vector<uint8_t>& vector, const uint8_t* data, const size_t size)
{
    for (size_t i = 0; i < size; i++)
    {
        vector.emplace_back(data[i]);
    }
}
