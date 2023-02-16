#include "PlayerStateWrapper.h"

void PlayerStateWrapper::WriteChangesToVector(std::vector<uint8_t>& changes, const EChangeTypeEntity& changeType) const
{
    switch (changeType)
    {
    case EChangeTypeEntity::PlayerInputState:
        EngineDefaults::EmplaceReplaceDataInVector(changes, &OldState.InputState);
        EngineDefaults::EmplaceReplaceDataInVector(changes, &State.InputState);
        break;
    case EChangeTypeEntity::PlayerCameraPitch:
        EngineDefaults::EmplaceReplaceDataInVector(changes, &OldState.CameraPitch);
        EngineDefaults::EmplaceReplaceDataInVector(changes, &State.CameraPitch);
        break;
    case EChangeTypeEntity::PlayerMode:
        EngineDefaults::EmplaceReplaceDataInVector(changes, &OldState.Mode);
        EngineDefaults::EmplaceReplaceDataInVector(changes, &State.Mode);
        break;
    case EChangeTypeEntity::PlayerCurrentSelectedBlock:
        EngineDefaults::EmplaceReplaceDataInVector(changes, &OldState.CurrentSelectedBlock);
        EngineDefaults::EmplaceReplaceDataInVector(changes, &State.CurrentSelectedBlock);
        break;
    default:
        LivingEntityStateWrapper::WriteChangesToVector(changes, changeType);
        break;
    }
}

void PlayerStateWrapper::ClearAllChanges(const EChangeTypeEntity& changeType)
{
    switch (changeType)
    {
    case EChangeTypeEntity::PlayerInputState:
        OldState.InputState = State.InputState;
        break;
    case EChangeTypeEntity::PlayerCameraPitch:
        OldState.CameraPitch = State.CameraPitch;
        break;
    case EChangeTypeEntity::PlayerMode:
        OldState.Mode = State.Mode;
        break;
    case EChangeTypeEntity::PlayerCurrentSelectedBlock:
        OldState.CurrentSelectedBlock = State.CurrentSelectedBlock;
        break;
    default:
        LivingEntityStateWrapper::ClearAllChanges(changeType);
        break;
    }
}

PlayerStateWrapper::PlayerStateWrapper(const uint16_t id) : LivingEntityStateWrapper(id, EEntityType::Player)
{
}

PlayerStateWrapper::PlayerStateWrapper(const PlayerState& otherState) : LivingEntityStateWrapper(otherState)
{
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

void PlayerStateWrapper::SetMode(const bool mode)
{
    State.Mode = mode;
    Changes.emplace(EChangeTypeEntity::PlayerMode);
}

void PlayerStateWrapper::SetCurrentSelectedBlock(const EBlockType currentSelectedBlock)
{
    State.CurrentSelectedBlock = currentSelectedBlock;
    Changes.emplace(EChangeTypeEntity::PlayerCurrentSelectedBlock);
}

void PlayerStateWrapper::WriteChangesToVector(std::vector<uint8_t>& changes) const
{
    LivingEntityStateWrapper::WriteChangesToVector(changes);
}

void PlayerStateWrapper::ClearAllChanges()
{
    LivingEntityStateWrapper::ClearAllChanges();
}
