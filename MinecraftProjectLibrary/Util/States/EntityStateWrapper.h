#pragma once
#include "EntityState.h"
#include "StateWrapper.h"

template <typename StateType> requires std::is_base_of_v<EntityState, StateType>
class EntityStateWrapper : public StateWrapper<StateType, EChangeTypeEntity>
{
protected:
    void ClearAllChanges(const EChangeTypeEntity& change) override;
    void WriteChangesToVector(std::vector<uint8_t>& changes, const EChangeTypeEntity& changeType) const override;

public:
    explicit EntityStateWrapper(uint16_t entityId, EEntityType entityType = EEntityType::Player);
    explicit EntityStateWrapper(const StateType& otherState);
    void WriteChangesToVector(std::vector<uint8_t>& changes) const override;
    void SetPosition(const glm::vec3& position);
    void SetRotation(const glm::vec3& rotation);
    void SetScale(const glm::vec3& scale);
    void SetIsGrounded(bool isGrounded);
    void SetVelocity(const glm::vec3& velocity);
    void ClearAllChanges() override;
};

template <typename StateType> requires std::is_base_of_v<EntityState, StateType>
void EntityStateWrapper<StateType>::ClearAllChanges(const EChangeTypeEntity& change)
{
    switch (change)
    {
    case EChangeTypeEntity::Position:
        this->OldState.EntityTransform.Position = this->State.EntityTransform.Position;
        break;
    case EChangeTypeEntity::Rotation:
        this->OldState.EntityTransform.Rotation = this->State.EntityTransform.Rotation;
        break;
    case EChangeTypeEntity::Scale:
        this->OldState.EntityTransform.Scale = this->State.EntityTransform.Scale;
        break;
    case EChangeTypeEntity::Velocity:
        this->OldState.EntityVelocity = this->State.EntityVelocity;
        break;
    case EChangeTypeEntity::IsGrounded:
        this->OldState.IsGrounded = this->State.IsGrounded;
        break;
    default:
        break;
    }
}

template <typename StateType> requires std::is_base_of_v<EntityState, StateType>
void EntityStateWrapper<StateType>::WriteChangesToVector(std::vector<uint8_t>& changes,
                                                         const EChangeTypeEntity& changeType) const
{
    switch (changeType)
    {
    case EChangeTypeEntity::Position:
        EngineDefaults::EmplaceReplaceDataInVector(changes, &this->OldState.EntityTransform.Position);
        EngineDefaults::EmplaceReplaceDataInVector(changes, &this->State.EntityTransform.Position);
        break;
    case EChangeTypeEntity::Rotation:
        EngineDefaults::EmplaceReplaceDataInVector(changes, &this->OldState.EntityTransform.Rotation);
        EngineDefaults::EmplaceReplaceDataInVector(changes, &this->State.EntityTransform.Rotation);
        break;
    case EChangeTypeEntity::Scale:
        EngineDefaults::EmplaceReplaceDataInVector(changes, &this->OldState.EntityTransform.Scale);
        EngineDefaults::EmplaceReplaceDataInVector(changes, &this->State.EntityTransform.Scale);
        break;
    case EChangeTypeEntity::Velocity:
        EngineDefaults::EmplaceReplaceDataInVector(changes, &this->OldState.EntityVelocity);
        EngineDefaults::EmplaceReplaceDataInVector(changes, &this->State.EntityVelocity);
        break;
    case EChangeTypeEntity::IsGrounded:
        EngineDefaults::EmplaceReplaceDataInVector(changes, &this->OldState.IsGrounded);
        EngineDefaults::EmplaceReplaceDataInVector(changes, &this->State.IsGrounded);
        break;
    default:
        break;
    }
}

template <typename StateType> requires std::is_base_of_v<EntityState, StateType>
EntityStateWrapper<StateType>::EntityStateWrapper(uint16_t entityId, EEntityType entityType)
{
    this->State.EntityId = entityId;
    this->OldState.EntityId = entityId;
    this->State.EntityType = entityType;
    this->OldState.EntityType = entityType;
}

template <typename StateType> requires std::is_base_of_v<EntityState, StateType>
EntityStateWrapper<StateType>::EntityStateWrapper(const StateType& otherState) : StateWrapper<StateType, EChangeTypeEntity>(otherState)
{
}

template <typename StateType> requires std::is_base_of_v<EntityState, StateType>
void EntityStateWrapper<StateType>::WriteChangesToVector(std::vector<uint8_t>& changes) const
{
    if (this->Changes.empty())
    {
        return;
    }
    switch (this->State.EntityType)
    {
    case EEntityType::Player:
        {
            constexpr auto changeType = EChangeType::PlayerState;
            EngineDefaults::EmplaceReplaceDataInVector(changes, &changeType);
            break;
        }
    case EEntityType::BlockBreakParticle:
        {
            constexpr auto changeType = EChangeType::BlockParticleState;
            EngineDefaults::EmplaceReplaceDataInVector(changes, &changeType);
            break;
        }
    default:
        break;
    }
    EngineDefaults::EmplaceReplaceDataInVector(changes, &this->State.EntityId);
    StateWrapper<StateType, EChangeTypeEntity>::WriteChangesToVector(changes);
}

template <typename StateType> requires std::is_base_of_v<EntityState, StateType>
void EntityStateWrapper<StateType>::SetPosition(const glm::vec3& position)
{
    this->State.EntityTransform.Position = position;
    this->Changes.emplace(EChangeTypeEntity::Position);
}

template <typename StateType> requires std::is_base_of_v<EntityState, StateType>
void EntityStateWrapper<StateType>::SetRotation(const glm::vec3& rotation)
{
    this->State.EntityTransform.Rotation = rotation;
    this->Changes.emplace(EChangeTypeEntity::Rotation);
}

template <typename StateType> requires std::is_base_of_v<EntityState, StateType>
void EntityStateWrapper<StateType>::SetScale(const glm::vec3& scale)
{
    this->State.EntityTransform.Scale = scale;
    this->Changes.emplace(EChangeTypeEntity::Scale);
}

template <typename StateType> requires std::is_base_of_v<EntityState, StateType>
void EntityStateWrapper<StateType>::SetIsGrounded(bool isGrounded)
{
    this->State.IsGrounded = isGrounded;
    this->Changes.emplace(EChangeTypeEntity::IsGrounded);
}

template <typename StateType> requires std::is_base_of_v<EntityState, StateType>
void EntityStateWrapper<StateType>::SetVelocity(const glm::vec3& velocity)
{
    this->State.EntityVelocity = velocity;
    this->Changes.emplace(EChangeTypeEntity::Velocity);
}

template <typename StateType> requires std::is_base_of_v<EntityState, StateType>
void EntityStateWrapper<StateType>::ClearAllChanges()
{
    StateWrapper<StateType, EChangeTypeEntity>::ClearAllChanges();
}
