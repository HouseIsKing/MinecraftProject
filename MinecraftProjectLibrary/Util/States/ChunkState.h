#pragma once
#include "Util/EngineDefaults.h"
#include <array>


struct ChunkState final : State
{
    ChunkCoords ChunkPosition;
    std::array<EBlockType, static_cast<size_t>(EngineDefaults::CHUNK_HEIGHT * EngineDefaults::CHUNK_DEPTH * EngineDefaults::CHUNK_WIDTH)> Blocks{EBlockType::Air};
    void Serialize(std::vector<uint8_t>& changes) const override;
    void Deserialize(const std::vector<uint8_t>& changes, size_t& pos) override;
};

inline void ChunkState::Serialize(std::vector<uint8_t>& changes) const
{
    EngineDefaults::EmplaceReplaceDataInVector(changes, &ChunkPosition);
    for (const EBlockType& block : Blocks)
    {
        EngineDefaults::EmplaceReplaceDataInVector(changes, &block);
    }
}

inline void ChunkState::Deserialize(const std::vector<uint8_t>& changes, size_t& pos)
{
    ChunkPosition = EngineDefaults::ReadDataFromVector<ChunkCoords>(changes, pos);
    for (EBlockType& block : Blocks)
    {
        block = EngineDefaults::ReadDataFromVector<EBlockType>(changes, pos);
    }
}
