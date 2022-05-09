//
// Created by amit on 4/22/2022.
//

#include "EngineDefaults.h"
#include "../World/Chunk.h"
#include <memory>

bool EngineDefaults::HasInit = false;

Shader* EngineDefaults::GetShader()
{
    if (!HasInit)
    {
        Init();
    }
    return TheShader.get();
}

void EngineDefaults::Init()
{
    HasInit = true;
    TheShader = std::make_unique<Shader>("Shaders/VertexShader.glsl", "Shaders/FragmentShader.glsl");
}

int EngineDefaults::GetChunkLocalIndex(int x, int y, int z)
{
    x %= Chunk::CHUNK_WIDTH;
    y %= Chunk::CHUNK_HEIGHT;
    z %= Chunk::CHUNK_DEPTH;
    return x + z * Chunk::CHUNK_WIDTH + y * Chunk::CHUNK_WIDTH * Chunk::CHUNK_DEPTH;
}

unique_ptr<Shader> EngineDefaults::TheShader = {};
CustomRandomEngine EngineDefaults::Engine = {};
