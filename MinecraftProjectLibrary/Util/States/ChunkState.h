#pragma once
#include "Util/EngineDefaults.h"
#include <array>


struct ChunkState : State
{
    ChunkCoords ChunkPosition;
    std::array<EBlockType, static_cast<size_t>(EngineDefaults::CHUNK_HEIGHT * EngineDefaults::CHUNK_DEPTH * EngineDefaults::CHUNK_WIDTH)> Blocks{EBlockType::Air};
};
