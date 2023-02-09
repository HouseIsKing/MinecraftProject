#pragma once
#include "Entity.h"
#include "Util/States/LivingEntityStateWrapper.h"
#include <glm/trigonometric.hpp>

template <typename Wrapper, typename StateType> requires std::is_base_of_v<LivingEntityStateWrapper<StateType>, Wrapper>
class LivingEntity : public Entity<Wrapper, StateType>
{
    void CalculateVelocity();

protected:
    LivingEntity(glm::vec3 entitySize, float x, float y, float z, const uint16_t& id);
    explicit LivingEntity(const StateType& state);
    void Tick() override;
    void ApplyRevertEntityChange(const std::vector<uint8_t>& changes, size_t& pos, EChangeTypeEntity change, bool revert = true) override;
};

template <typename Wrapper, typename StateType> requires std::is_base_of_v<LivingEntityStateWrapper<StateType>, Wrapper>
void LivingEntity<Wrapper, StateType>::CalculateVelocity()
{
    const StateType& state = this->State.GetState();
    glm::vec3 entityVelocity = state.EntityVelocity;
    float speedModifier = 0.1F;
    if (state.JumpRequested && state.IsGrounded)
    {
        entityVelocity.y = 0.5F;
    }
    if (!state.IsGrounded)
    {
        speedModifier *= 0.2F;
    }
    entityVelocity.y -= 0.08F;
    const glm::vec3 rotation = state.EntityTransform.Rotation;
    if (const float d = sqrt(state.HorizontalInput * state.HorizontalInput + state.VerticalInput * state.VerticalInput); d > 0.001F)
    {
        speedModifier /= d;
        entityVelocity.x += speedModifier * (state.VerticalInput * cos(glm::radians(rotation.y)) - state.HorizontalInput * sin(glm::radians(rotation.y)));
        entityVelocity.z += speedModifier * (state.HorizontalInput * cos(glm::radians(rotation.y)) + state.VerticalInput * sin(glm::radians(rotation.y)));
    }
    this->State.SetVelocity(entityVelocity);
    Entity<Wrapper, StateType>::CheckCollisionAndMove();
    entityVelocity = state.EntityVelocity;
    entityVelocity.x *= 0.91F;
    entityVelocity.z *= 0.91F;
    entityVelocity.y *= 0.98F;
    if (state.IsGrounded)
    {
        entityVelocity.x *= 0.7F;
        entityVelocity.z *= 0.7F;
    }
    this->State.SetVelocity(entityVelocity);
}

template <typename Wrapper, typename StateType> requires std::is_base_of_v<LivingEntityStateWrapper<StateType>, Wrapper>
LivingEntity<Wrapper, StateType>::LivingEntity(glm::vec3 entitySize, const float x, const float y, const float z, const uint16_t& id) : Entity<Wrapper, StateType>(entitySize, glm::vec3(x, y, z), id)
{
}

template <typename Wrapper, typename StateType> requires std::is_base_of_v<LivingEntityStateWrapper<StateType>, Wrapper>
LivingEntity<Wrapper, StateType>::LivingEntity(const StateType& state) : Entity<Wrapper, StateType>(state)
{
}

template <typename Wrapper, typename StateType> requires std::is_base_of_v<LivingEntityStateWrapper<StateType>, Wrapper>
void LivingEntity<Wrapper, StateType>::Tick()
{
    Entity<Wrapper, StateType>::Tick();
    CalculateVelocity();
}

template <typename Wrapper, typename StateType> requires std::is_base_of_v<LivingEntityStateWrapper<StateType>, Wrapper>
void LivingEntity<Wrapper, StateType>::ApplyRevertEntityChange(const std::vector<uint8_t>& changes, size_t& pos,
                                                               EChangeTypeEntity change, bool revert)
{
    switch (change)
    {
    case EChangeTypeEntity::HorizontalInput:
        {
            if (revert)
            {
                this->State.SetHorizontalInput(EngineDefaults::ReadDataFromVector<float>(changes, pos));
                pos += sizeof(float);
            }
            else
            {
                pos += sizeof(float);
                this->State.SetHorizontalInput(EngineDefaults::ReadDataFromVector<float>(changes, pos));
            }
            break;
        }
    case EChangeTypeEntity::VerticalInput:
        {
            if (revert)
            {
                this->State.SetVerticalInput(EngineDefaults::ReadDataFromVector<float>(changes, pos));
                pos += sizeof(float);
            }
            else
            {
                pos += sizeof(float);
                this->State.SetVerticalInput(EngineDefaults::ReadDataFromVector<float>(changes, pos));
            }
            break;
        }
    case EChangeTypeEntity::JumpRequested:
        {
            if (revert)
            {
                this->State.SetJumpRequested(EngineDefaults::ReadDataFromVector<bool>(changes, pos));
                pos += sizeof(bool);
            }
            else
            {
                pos += sizeof(bool);
                this->State.SetJumpRequested(EngineDefaults::ReadDataFromVector<bool>(changes, pos));
            }
            break;
        }
    default:
        Entity<Wrapper, StateType>::ApplyRevertEntityChange(changes, pos, change, revert);
        break;
    }
}
