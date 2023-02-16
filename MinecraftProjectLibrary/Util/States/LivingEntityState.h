#pragma once
#include "EntityState.h"

struct LivingEntityState : EntityState
{
    bool JumpRequested{false};
    float HorizontalInput{0.0F};
    float VerticalInput{0.0F};
    void Serialize(std::vector<uint8_t>& changes) const override;
    void Deserialize(const std::vector<uint8_t>& changes, size_t& pos) override;
};

inline void LivingEntityState::Serialize(std::vector<uint8_t>& changes) const
{
    EntityState::Serialize(changes);
    EngineDefaults::EmplaceReplaceDataInVector(changes, &JumpRequested);
    EngineDefaults::EmplaceReplaceDataInVector(changes, &HorizontalInput);
    EngineDefaults::EmplaceReplaceDataInVector(changes, &VerticalInput);
}

inline void LivingEntityState::Deserialize(const std::vector<uint8_t>& changes, size_t& pos)
{
    EntityState::Deserialize(changes, pos);
    JumpRequested = EngineDefaults::ReadDataFromVector<bool>(changes, pos);
    HorizontalInput = EngineDefaults::ReadDataFromVector<float>(changes, pos);
    VerticalInput = EngineDefaults::ReadDataFromVector<float>(changes, pos);
}
