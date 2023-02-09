#pragma once
#include "Entities/Player.h"
#include "Util/CustomRandomEngine.h"
#include "World/Chunk.h"
#include <map>

struct WorldState : State
{
    std::map<ChunkCoords, Chunk> Chunks{};
    std::map<uint16_t, Player> Players{};
    std::map<glm::ivec2, uint8_t, IVec2Comparator> Lights{};
    uint64_t WorldTime{};
    CustomRandomEngine RandomEngine{};
};
