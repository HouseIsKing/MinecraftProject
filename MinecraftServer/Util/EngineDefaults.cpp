#include "EngineDefaults.h"
#include "World/Chunk.h"

CustomRandomEngine EngineDefaults::Engine = {};

float EngineDefaults::ConvertLightLevelToAmbient(const int lightLevel)
{
    if (lightLevel == 0)
    {
        return 0.6F;
    }
    return 1.0F;
}

float EngineDefaults::GetNextFloat()
{
    return static_cast<float>(static_cast<double>(CustomRandomEngine::GetNext()) / CustomRandomEngine::M);
}

int EngineDefaults::GetChunkLocalIndex(int x, int y, int z)
{
    x %= Chunk::CHUNK_WIDTH;
    y %= Chunk::CHUNK_HEIGHT;
    z %= Chunk::CHUNK_DEPTH;
    return x + z * Chunk::CHUNK_WIDTH + y * Chunk::CHUNK_WIDTH * Chunk::CHUNK_DEPTH;
}
