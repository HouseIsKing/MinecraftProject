#include "LivingEntity.h"

LivingEntity::LivingEntity(const glm::vec3 entitySize, const float x, const float y, const float z, LivingEntityState* state) : Entity(entitySize,
    x, y, z, state)
{
}

void LivingEntity::CalculateVelocity() const
{
    const auto* state = reinterpret_cast<LivingEntityState*>(this->State.get());
    float speedModifier = 0.1F;
    if (state->JumpRequested && this->State->IsGrounded)
    {
        this->State->EntityVelocity.y = 0.5F;
    }
    if (!this->State->IsGrounded)
    {
        speedModifier *= 0.2F;
    }
    this->State->EntityVelocity.y -= 0.08F;
    const glm::vec3 rotation = this->GetTransform().Rotation;
    if (const float d = sqrt(state->HorizontalInput * state->HorizontalInput + state->VerticalInput * state->VerticalInput); d > 0.001F)
    {
        speedModifier /= d;
        this->State->EntityVelocity.x += speedModifier * (state->VerticalInput * cos(glm::radians(rotation.y)) - state->HorizontalInput * sin(glm::radians(rotation.y)));
        this->State->EntityVelocity.z += speedModifier * (state->HorizontalInput * cos(glm::radians(rotation.y)) + state->VerticalInput * sin(glm::radians(rotation.y)));
    }
    this->CheckCollisionAndMove();
    this->State->EntityVelocity.x *= 0.91F;
    this->State->EntityVelocity.z *= 0.91F;
    this->State->EntityVelocity.y *= 0.98F;
    if (this->State->IsGrounded)
    {
        this->State->EntityVelocity.x *= 0.7F;
        this->State->EntityVelocity.z *= 0.7F;
    }
}

void LivingEntity::Tick()
{
    Entity::Tick();
    CalculateVelocity();
}

EntityState* LivingEntity::GetEntityState() const
{
    return new LivingEntityState(*reinterpret_cast<LivingEntityState*>(this->State.get()));
}

void LivingEntity::ApplyEntityChange(const std::vector<uint8_t>& changes, size_t& pos, EChangeTypeEntity change)
{
    switch (change)
    {
    case EChangeTypeEntity::HorizontalInput:
        {
            reinterpret_cast<LivingEntityState*>(this->State.get())->HorizontalInput = *reinterpret_cast<const float*>(&changes[pos]);
            pos += sizeof(float);
            break;
        }
    case EChangeTypeEntity::VerticalInput:
        {
            reinterpret_cast<LivingEntityState*>(this->State.get())->VerticalInput = *reinterpret_cast<const float*>(&changes[pos]);
            pos += sizeof(float);
            break;
        }
    case EChangeTypeEntity::JumpRequested:
        {
            reinterpret_cast<LivingEntityState*>(this->State.get())->JumpRequested = *reinterpret_cast<const bool*>(&changes[pos]);
            pos += sizeof(bool);
            break;
        }
    default:
        Entity::ApplyEntityChange(changes, pos, change);
        break;
    }
}