#pragma once
#include "LivingEntityState.h"

struct PlayerState final : LivingEntityState
{
    PlayerState();
    ClientInputStruct InputState{};
    float CameraPitch{0.0F};
    bool Mode{false};
    EBlockType CurrentSelectedBlock{EBlockType::Stone};
    void Serialize(std::vector<uint8_t>& changes) const override;
    void Deserialize(const std::vector<uint8_t>& changes, size_t& pos) override;
};

inline PlayerState::PlayerState()
{
    EntityType = EEntityType::Player;
    EntityTransform.Scale = EngineDefaults::PLAYER_SIZE;
}

inline void PlayerState::Serialize(std::vector<uint8_t>& changes) const
{
    LivingEntityState::Serialize(changes);
    EngineDefaults::EmplaceReplaceDataInVector(changes, &InputState);
    EngineDefaults::EmplaceReplaceDataInVector(changes, &CameraPitch);
    EngineDefaults::EmplaceReplaceDataInVector(changes, &Mode);
    EngineDefaults::EmplaceReplaceDataInVector(changes, &CurrentSelectedBlock);
}

inline void PlayerState::Deserialize(const std::vector<uint8_t>& changes, size_t& pos)
{
    LivingEntityState::Deserialize(changes, pos);
    InputState = EngineDefaults::ReadDataFromVector<ClientInputStruct>(changes, pos);
    CameraPitch = EngineDefaults::ReadDataFromVector<float>(changes, pos);
    Mode = EngineDefaults::ReadDataFromVector<bool>(changes, pos);
    CurrentSelectedBlock = EngineDefaults::ReadDataFromVector<EBlockType>(changes, pos);
}
