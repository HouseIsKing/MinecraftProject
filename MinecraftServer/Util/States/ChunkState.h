#pragma once
#include "Util/EngineDefaults.h"
#include "World/Blocks/BlockTypeList.h"
#include "World/ChunkCoords.h"
#include <array>

struct ChunkState
{
    ChunkCoords ChunkPosition;
    std::array<EBlockType, static_cast<size_t>(EngineDefaults::CHUNK_HEIGHT * EngineDefaults::CHUNK_DEPTH * EngineDefaults::CHUNK_WIDTH)> Blocks{EBlockType::Air};
    std::vector<uint8_t> operator-(const ChunkState& other) const;
};

inline std::vector<uint8_t> ChunkState::operator-(const ChunkState& other) const
{
    std::vector<uint8_t> result{};
    EngineDefaults::EmplaceDataInVector(result, reinterpret_cast<const uint8_t*>(&ChunkPosition), sizeof ChunkCoords);
    result.emplace_back();
    result.emplace_back();
    for (uint16_t i = 0; static_cast<size_t>(i) < Blocks.size(); i++)
    {
        if (Blocks[i] != other.Blocks[i])
        {
            result[sizeof ChunkPosition]++;
            if (result[sizeof ChunkPosition] == 0)
            {
                result[1 + sizeof ChunkPosition]++;
            }
            EngineDefaults::EmplaceDataInVector(result, reinterpret_cast<const uint8_t*>(&i), sizeof(uint16_t));
            EngineDefaults::EmplaceDataInVector(result, reinterpret_cast<const uint8_t*>(&other.Blocks[i]), sizeof(EBlockType));
        }
    }
    return result;
}
