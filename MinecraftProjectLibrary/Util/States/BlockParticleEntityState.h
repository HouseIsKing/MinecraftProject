#pragma once
#include "EntityState.h"

struct BlockParticleEntityState final : EntityState
{
    BlockParticleEntityState();
    uint8_t LifeTime{0};
    uint8_t Age{0};
    EBlockType BlockParticleType{EBlockType::Air};
    void Serialize(std::vector<uint8_t>& changes) const override;
    void Deserialize(const std::vector<uint8_t>& changes, size_t& pos) override;
};

inline BlockParticleEntityState::BlockParticleEntityState()
{
    EntityType = EEntityType::BlockBreakParticle;
}

inline void BlockParticleEntityState::Serialize(std::vector<uint8_t>& changes) const
{
    EntityState::Serialize(changes);
    EngineDefaults::EmplaceReplaceDataInVector(changes, &LifeTime);
    EngineDefaults::EmplaceReplaceDataInVector(changes, &Age);
    EngineDefaults::EmplaceReplaceDataInVector(changes, &BlockParticleType);
}

inline void BlockParticleEntityState::Deserialize(const std::vector<uint8_t>& changes, size_t& pos)
{
    EntityState::Deserialize(changes, pos);
    LifeTime = EngineDefaults::ReadDataFromVector<uint8_t>(changes, pos);
    Age = EngineDefaults::ReadDataFromVector<uint8_t>(changes, pos);
    BlockParticleType = EngineDefaults::ReadDataFromVector<EBlockType>(changes, pos);
}
