#pragma once
#include "ClientInputState.h"
#include "LivingEntityState.h"

struct PlayerState final : LivingEntityState
{
    ClientInputState InputState;
    float CameraPitch{0.0F};
    explicit PlayerState(uint16_t id);
    std::vector<uint8_t> operator-(const EntityState& newState) const override;
    [[nodiscard]] size_t GetSize() const override;
};

inline PlayerState::PlayerState(const uint16_t id) : LivingEntityState(id)
{
}

inline std::vector<uint8_t> PlayerState::operator-(const EntityState& newState) const
{
    std::vector<uint8_t> result = LivingEntityState::operator-(newState);
    const auto& newStatePlayer = dynamic_cast<const PlayerState&>(newState);
    std::vector<uint8_t> temp = InputState - newStatePlayer.InputState;
    if (abs(CameraPitch - newStatePlayer.CameraPitch) > 0.001F)
    {
        temp[0]++;
        constexpr auto change = EChangeTypePlayer::CameraPitch;
        EngineDefaults::EmplaceDataInVector(temp, reinterpret_cast<const uint8_t*>(&change), sizeof(EChangeTypeEntity));
        EngineDefaults::EmplaceDataInVector(temp, reinterpret_cast<const uint8_t*>(&newStatePlayer.CameraPitch), sizeof CameraPitch);
    }
    if (temp[0] > 0)
    {
        result[sizeof EntityId]++;
        constexpr auto change = EChangeTypeEntity::PlayerChange;
        EngineDefaults::EmplaceDataInVector(result, reinterpret_cast<const uint8_t*>(&change), sizeof(EChangeTypeEntity));
        EngineDefaults::EmplaceDataInVector(result, temp.data(), temp.size());
    }
    return result;
}

inline size_t PlayerState::GetSize() const
{
    return sizeof PlayerState;
}
