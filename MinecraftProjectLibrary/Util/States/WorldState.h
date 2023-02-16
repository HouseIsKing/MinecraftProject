#pragma once
#include "Entities/Player.h"
#include "Util/CustomRandomEngine.h"
#include "World/Chunk.h"
#include <map>

#include "Entities/BlockParticleEntity.h"

struct WorldState final : State
{
    std::map<ChunkCoords, Chunk*> Chunks{};
    std::map<uint16_t, Player*> Players{};
    std::map<uint16_t, BlockParticleEntity*> BlockParticleEntities{};
    std::map<glm::ivec2, uint8_t, IVec2Comparator> Lights{};
    uint64_t WorldTime{};
    CustomRandomEngine RandomEngine{};

private:
    void Serialize(std::vector<uint8_t>& changes) const override;
    void Deserialize(const std::vector<uint8_t>& changes, size_t& pos) override;
};

inline void WorldState::Serialize(std::vector<uint8_t>& /*changes*/) const
{
}

inline void WorldState::Deserialize(const std::vector<uint8_t>& /*changes*/, size_t& /*pos*/)
{
}
