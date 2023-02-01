#pragma once
#include "EntityState.h"
#include "Util/EngineDefaults.h"

struct LivingEntityState : EntityState
{
    bool JumpRequested{false};
    float HorizontalInput{0.0F};
    float VerticalInput{0.0F};
    LivingEntityState(uint16_t id, EEntityType type = EEntityType::Player);
    [[nodiscard]] size_t GetSize() const override;
    std::vector<uint8_t> operator-(const EntityState& newState) const override;
};

inline LivingEntityState::LivingEntityState(const uint16_t id, const EEntityType type) : EntityState(id, type)
{
}

inline size_t LivingEntityState::GetSize() const
{
    return sizeof LivingEntityState;
}

inline std::vector<uint8_t> LivingEntityState::operator-(const EntityState& newState) const
{
    std::vector<uint8_t> result = EntityState::operator-(newState);
    const auto& newStateLiving = dynamic_cast<const LivingEntityState&>(newState);
    if (JumpRequested != newStateLiving.JumpRequested)
    {
        result[sizeof EntityId]++;
        constexpr auto temp = EChangeTypeEntity::JumpRequested;
        EngineDefaults::EmplaceDataInVector(result, reinterpret_cast<const uint8_t*>(&temp), sizeof(EChangeTypeEntity));
        EngineDefaults::EmplaceDataInVector(result, reinterpret_cast<const uint8_t*>(&newStateLiving.JumpRequested), sizeof(JumpRequested));
    }
    if (abs(HorizontalInput - newStateLiving.HorizontalInput) > 0.001F)
    {
        result[sizeof EntityId]++;
        constexpr auto temp = EChangeTypeEntity::HorizontalInput;
        EngineDefaults::EmplaceDataInVector(result, reinterpret_cast<const uint8_t*>(&temp), sizeof(EChangeTypeEntity));
        EngineDefaults::EmplaceDataInVector(result, reinterpret_cast<const uint8_t*>(&newStateLiving.HorizontalInput), sizeof(HorizontalInput));
    }
    if (abs(VerticalInput - newStateLiving.VerticalInput) > 0.001F)
    {
        result[sizeof EntityId]++;
        constexpr auto temp = EChangeTypeEntity::VerticalInput;
        EngineDefaults::EmplaceDataInVector(result, reinterpret_cast<const uint8_t*>(&temp), sizeof(EChangeTypeEntity));
        EngineDefaults::EmplaceDataInVector(result, reinterpret_cast<const uint8_t*>(&newStateLiving.VerticalInput), sizeof(VerticalInput));
    }
    return result;
}
