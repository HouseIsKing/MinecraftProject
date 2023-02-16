#include "BlockParticleEntity.h"

#include <glm/detail/func_geometric.inl>

#include "Util/States/PlayerState.h"
#include "World/World.h"

BlockParticleEntity::BlockParticleEntity(const BlockParticleEntityState& state) : Entity(state)
{
}

void BlockParticleEntity::ApplyRevertEntityChange(const std::vector<uint8_t>& changes, size_t& pos,
                                                  const EChangeTypeEntity change, const bool revert)
{
    switch (change)
    {
    case EChangeTypeEntity::BlockParticleAge:
        if (revert)
        {
            State.SetAge(EngineDefaults::ReadDataFromVector<uint8_t>(changes, pos));
            pos += sizeof(uint8_t);
        }
        else
        {
            pos += sizeof(uint8_t);
            State.SetAge(EngineDefaults::ReadDataFromVector<uint8_t>(changes, pos));
        }
        break;
    case EChangeTypeEntity::BlockParticleLifeTime:
        if (revert)
        {
            State.SetLifeTime(EngineDefaults::ReadDataFromVector<uint8_t>(changes, pos));
            pos += sizeof uint8_t;
        }
        else
        {
            pos += sizeof uint8_t;
            State.SetLifeTime(EngineDefaults::ReadDataFromVector<uint8_t>(changes, pos));
        }
        break;
    default:
        Entity::ApplyRevertEntityChange(changes, pos, change, revert);
        break;
    }
}

void BlockParticleEntity::Tick()
{
    if (State.GetState().Age >= State.GetState().LifeTime)
    {
        World::GetWorld()->EntitiesToRemove.emplace_back(State.GetState().EntityId);
        return;
    }
    Entity::Tick();
    glm::vec3 stateEntityVelocity = State.GetState().EntityVelocity;
    State.SetAge(State.GetState().Age + 1);
    stateEntityVelocity.y -= 0.06F;
    State.SetVelocity(stateEntityVelocity);
    this->CheckCollisionAndMove();
    stateEntityVelocity = State.GetState().EntityVelocity;
    stateEntityVelocity.x *= 0.98F;
    stateEntityVelocity.y *= 0.98F;
    stateEntityVelocity.z *= 0.98F;
    if (State.GetState().IsGrounded)
    {
        stateEntityVelocity.x *= 0.7F;
        stateEntityVelocity.z *= 0.7F;
    }
    State.SetVelocity(stateEntityVelocity);
}
