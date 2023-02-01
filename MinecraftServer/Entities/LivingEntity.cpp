#include "LivingEntity.h"
#include <glm/trigonometric.hpp>

LivingEntity::LivingEntity(const glm::vec3 entitySize, const float x, const float y, const float z, LivingEntityState* state) : Entity(entitySize,
    x, y, z, state)
{
}

void LivingEntity::CalculateVelocity() const
{
    const auto* state = reinterpret_cast<LivingEntityState*>(State.get());
    float speedModifier = 0.1F;
    if (state->JumpRequested && State->IsGrounded)
    {
        State->EntityVelocity.y = 0.5F;
    }
    if (!State->IsGrounded)
    {
        speedModifier *= 0.2F;
    }
    State->EntityVelocity.y -= 0.08F;
    const glm::vec3 rotation = GetTransform().Rotation;
    if (const float d = sqrt(state->HorizontalInput * state->HorizontalInput + state->VerticalInput * state->VerticalInput); d > 0.001F)
    {
        speedModifier /= d;
        State->EntityVelocity.x += speedModifier * (state->VerticalInput * cos(glm::radians(rotation.y)) - state->HorizontalInput * sin(glm::radians(rotation.y)));
        State->EntityVelocity.z += speedModifier * (state->HorizontalInput * cos(glm::radians(rotation.y)) + state->VerticalInput * sin(glm::radians(rotation.y)));
    }
    CheckCollisionAndMove();
    State->EntityVelocity.x *= 0.91F;
    State->EntityVelocity.z *= 0.91F;
    State->EntityVelocity.y *= 0.98F;
    if (State->IsGrounded)
    {
        State->EntityVelocity.x *= 0.7F;
        State->EntityVelocity.z *= 0.7F;
    }
}

void LivingEntity::Tick()
{
    Entity::Tick();
    CalculateVelocity();
}

EntityState* LivingEntity::GetEntityState() const
{
    return new LivingEntityState(*reinterpret_cast<LivingEntityState*>(State.get()));
}

void LivingEntity::ApplyEntityChange(const std::vector<uint8_t>& changes, size_t& pos, const EChangeTypeEntity change)
{
    switch (change)
    {
    case EChangeTypeEntity::HorizontalInput:
        {
            reinterpret_cast<LivingEntityState*>(State.get())->HorizontalInput = *reinterpret_cast<const float*>(&changes[pos]);
            pos += sizeof(float);
            break;
        }
    case EChangeTypeEntity::VerticalInput:
        {
            reinterpret_cast<LivingEntityState*>(State.get())->VerticalInput = *reinterpret_cast<const float*>(&changes[pos]);
            pos += sizeof(float);
            break;
        }
    case EChangeTypeEntity::JumpRequested:
        {
            reinterpret_cast<LivingEntityState*>(State.get())->JumpRequested = *reinterpret_cast<const bool*>(&changes[pos]);
            pos += sizeof(bool);
            break;
        }
    default:
        Entity::ApplyEntityChange(changes, pos, change);
        break;
    }
}
