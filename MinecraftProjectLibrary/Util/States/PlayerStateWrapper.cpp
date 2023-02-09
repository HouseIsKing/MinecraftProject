#include "PlayerStateWrapper.h"

PlayerStateWrapper::PlayerStateWrapper(const uint16_t id) : LivingEntityStateWrapper(id, EEntityType::Player)
{
}

PlayerStateWrapper::PlayerStateWrapper(const PlayerState& otherState) : LivingEntityStateWrapper(otherState)
{
}

void PlayerStateWrapper::WriteChangesToVector(std::vector<uint8_t>& changes) const
{
    LivingEntityStateWrapper::WriteChangesToVector(changes);
    for (const EChangeTypeEntity& change : Changes)
    {
        switch (change)
        {
        case EChangeTypeEntity::PlayerInputState:
            EngineDefaults::EmplaceReplaceDataInVector(changes, &change);
            EngineDefaults::EmplaceReplaceDataInVector(changes, &OldState.InputState);
            EngineDefaults::EmplaceReplaceDataInVector(changes, &State.InputState);
            break;
        case EChangeTypeEntity::PlayerCameraPitch:
            EngineDefaults::EmplaceReplaceDataInVector(changes, &change);
            EngineDefaults::EmplaceReplaceDataInVector(changes, &OldState.CameraPitch);
            EngineDefaults::EmplaceReplaceDataInVector(changes, &State.CameraPitch);
            break;
        default:
            break;
        }
    }
}

void PlayerStateWrapper::ClearAllChanges()
{
    for (const EChangeTypeEntity& change : Changes)
    {
        switch (change)
        {
        case EChangeTypeEntity::PlayerInputState:
            OldState.InputState = State.InputState;
            break;
        case EChangeTypeEntity::PlayerCameraPitch:
            OldState.CameraPitch = State.CameraPitch;
            break;
        default:
            break;
        }
    }
    LivingEntityStateWrapper::ClearAllChanges();
}

void PlayerStateWrapper::SetInputState(const ClientInputStruct& inputState)
{
    State.InputState = inputState;
    Changes.emplace(EChangeTypeEntity::PlayerInputState);
}

void PlayerStateWrapper::SetCameraPitch(const float cameraPitch)
{
    State.CameraPitch = cameraPitch;
    Changes.emplace(EChangeTypeEntity::PlayerCameraPitch);
}
