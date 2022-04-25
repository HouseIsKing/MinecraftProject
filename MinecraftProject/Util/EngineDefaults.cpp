//
// Created by amit on 4/22/2022.
//

#include "EngineDefaults.h"

#include <memory>

bool EngineDefaults::hasInit = false;
Shader* EngineDefaults::getShader()
{
    if(!hasInit) {
        init();
    }
    return theShader.get();
}

void EngineDefaults::init() {
    hasInit = true;
    theShader = std::make_unique<Shader>("Shaders/VertexShader.glsl", "Shaders/FragmentShader.glsl");
}

int EngineDefaults::getChunkLocalIndex(int x, int y, int z) {
    x %= Chunk::CHUNK_WIDTH;
    y %= Chunk::CHUNK_HEIGHT;
    z %= Chunk::CHUNK_DEPTH;
    return x + z * Chunk::CHUNK_WIDTH + y * Chunk::CHUNK_WIDTH * Chunk::CHUNK_DEPTH;
}

unique_ptr<Shader> EngineDefaults::theShader = {};

PosBlock::PosBlock(int x, int y, int z, EBlockType type) : x(x), y(y), z(z), type(type)
{
}
