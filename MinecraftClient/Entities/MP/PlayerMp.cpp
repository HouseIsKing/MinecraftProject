#include "PlayerMp.h"

PlayerMp::PlayerMp(const PlayerState& state) : PlayerController(state)
{
}

void PlayerMp::Tick()
{
    InputState = InputStatesHistory[TheWorld->GetWorldTime() % EngineDefaults::ROLLBACK_COUNT];
    PlayerController::Tick();
}

const ClientInputState& PlayerMp::GetInputState(const size_t tickOffset) const
{
    return InputStatesHistory[tickOffset % EngineDefaults::ROLLBACK_COUNT];
}

void PlayerMp::CopyInputStateToHistory(const size_t tickOffset)
{
    InputStatesHistory[tickOffset % EngineDefaults::ROLLBACK_COUNT] = InputState;
}

EntityState* PlayerMp::GetEntityState() const
{
    auto* state = new PlayerState(*reinterpret_cast<PlayerState*>(State.get()));
    state->InputState = InputStatesHistory[TheWorld->GetWorldTime() % EngineDefaults::ROLLBACK_COUNT];
    return state;
}

void PlayerMp::ApplyEntityChange(const std::vector<uint8_t>& changes, size_t& pos, EChangeTypeEntity change)
{
    switch (change)
    {
    case EChangeTypeEntity::PlayerChange:
        {
            auto* state = reinterpret_cast<PlayerState*>(State.get());
            const uint8_t changesCount = changes[pos];
            pos += sizeof(uint8_t);
            for (uint8_t i = 0; i < changesCount; i++)
            {
                const EChangeTypePlayer changeType = *reinterpret_cast<const EChangeTypePlayer*>(&changes[pos]);
                pos += sizeof(EChangeTypePlayer);
                switch (changeType)
                {
                case EChangeTypePlayer::Jump:
                    {
                        state->InputState.JumpPressed = *reinterpret_cast<const bool*>(&changes[pos]);
                        pos += sizeof(bool);
                        break;
                    }
                case EChangeTypePlayer::FivePressed:
                    {
                        state->InputState.FivePressed = *reinterpret_cast<const bool*>(&changes[pos]);
                        pos += sizeof(bool);
                        break;
                    }
                case EChangeTypePlayer::ForwardAxis:
                    {
                        state->InputState.ForwardAxis = *reinterpret_cast<const int8_t*>(&changes[pos]);
                        pos += sizeof(uint8_t);
                        break;
                    }
                case EChangeTypePlayer::RightAxis:
                    {
                        state->InputState.RightAxis = *reinterpret_cast<const int8_t*>(&changes[pos]);
                        pos += sizeof(uint8_t);
                        break;
                    }
                case EChangeTypePlayer::MouseX:
                    {
                        state->InputState.MouseX = *reinterpret_cast<const float*>(&changes[pos]);
                        pos += sizeof(float);
                        break;
                    }
                case EChangeTypePlayer::MouseY:
                    {
                        state->InputState.MouseY = *reinterpret_cast<const float*>(&changes[pos]);
                        pos += sizeof(float);
                        break;
                    }
                case EChangeTypePlayer::CameraPitch:
                    {
                        state->CameraPitch = *reinterpret_cast<const float*>(&changes[pos]);
                        pos += sizeof(float);
                        break;
                    }
                case EChangeTypePlayer::FourPressed:
                    {
                        state->InputState.FourPressed = *reinterpret_cast<const bool*>(&changes[pos]);
                        pos += sizeof(bool);
                        break;
                    }
                case EChangeTypePlayer::OnePressed:
                    {
                        state->InputState.OnePressed = *reinterpret_cast<const bool*>(&changes[pos]);
                        pos += sizeof(bool);
                        break;
                    }
                case EChangeTypePlayer::TwoPressed:
                    {
                        state->InputState.TwoPressed = *reinterpret_cast<const bool*>(&changes[pos]);
                        pos += sizeof(bool);
                        break;
                    }
                case EChangeTypePlayer::ThreePressed:
                    {
                        state->InputState.ThreePressed = *reinterpret_cast<const bool*>(&changes[pos]);
                        pos += sizeof(bool);
                        break;
                    }
                case EChangeTypePlayer::ResetPos:
                    {
                        state->InputState.ResetPressed = *reinterpret_cast<const bool*>(&changes[pos]);
                        pos += sizeof(bool);
                        break;
                    }
                case EChangeTypePlayer::SpawnZombie:
                    {
                        state->InputState.SpawnZombiePressed = *reinterpret_cast<const bool*>(&changes[pos]);
                        pos += sizeof(bool);
                        break;
                    }
                case EChangeTypePlayer::LeftMouseButton:
                    {
                        state->InputState.LeftMouseButtonPressed = *reinterpret_cast<const bool*>(&changes[pos]);
                        pos += sizeof(bool);
                        break;
                    }
                case EChangeTypePlayer::RightMouseButton:
                    {
                        state->InputState.RightMouseButtonPressed = *reinterpret_cast<const bool*>(&changes[pos]);
                        pos += sizeof(bool);
                        break;
                    }
                }
            }
            break;
        }
    default:
        PlayerController::ApplyEntityChange(changes, pos, change);
        break;
    }
}
