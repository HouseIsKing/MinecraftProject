#pragma once
#include "EntityStateWrapper.h"
#include "LivingEntityState.h"

template <typename LivingStateType> requires std::is_base_of_v<LivingEntityState, LivingStateType>
class LivingEntityStateWrapper : public EntityStateWrapper<LivingStateType>
{
protected:
    void ClearAllChanges(const EChangeTypeEntity& change) override;
    void WriteChangesToVector(std::vector<uint8_t>& changes, const EChangeTypeEntity& change) const override;

public:
    explicit LivingEntityStateWrapper(uint16_t id, EEntityType entityType = EEntityType::Player);
    explicit LivingEntityStateWrapper(const LivingStateType& otherState);
    void SetHorizontalInput(float horizontalInput);
    void SetVerticalInput(float verticalInput);
    void SetJumpRequested(bool jumpRequested);
    void WriteChangesToVector(std::vector<uint8_t>& changes) const override;
    void ClearAllChanges() override;
};

template <typename LivingStateType> requires std::is_base_of_v<LivingEntityState, LivingStateType>
void LivingEntityStateWrapper<LivingStateType>::ClearAllChanges(const EChangeTypeEntity& change)
{
    switch (change)
    {
    case EChangeTypeEntity::HorizontalInput:
        this->OldState.HorizontalInput = this->State.HorizontalInput;
        break;
    case EChangeTypeEntity::VerticalInput:
        this->OldState.VerticalInput = this->State.VerticalInput;
        break;
    case EChangeTypeEntity::JumpRequested:
        this->OldState.JumpRequested = this->State.JumpRequested;
        break;
    default:
        EntityStateWrapper<LivingStateType>::ClearAllChanges(change);
        break;
    }
}

template <typename LivingStateType> requires std::is_base_of_v<LivingEntityState, LivingStateType>
void LivingEntityStateWrapper<LivingStateType>::WriteChangesToVector(std::vector<uint8_t>& changes,
                                                                     const EChangeTypeEntity& change) const
{
    switch (change)
    {
    case EChangeTypeEntity::HorizontalInput:
        EngineDefaults::EmplaceReplaceDataInVector(changes, &this->OldState.HorizontalInput);
        EngineDefaults::EmplaceReplaceDataInVector(changes, &this->State.HorizontalInput);
        break;
    case EChangeTypeEntity::VerticalInput:
        EngineDefaults::EmplaceReplaceDataInVector(changes, &this->OldState.VerticalInput);
        EngineDefaults::EmplaceReplaceDataInVector(changes, &this->State.VerticalInput);
        break;
    case EChangeTypeEntity::JumpRequested:
        EngineDefaults::EmplaceReplaceDataInVector(changes, &this->OldState.JumpRequested);
        EngineDefaults::EmplaceReplaceDataInVector(changes, &this->State.JumpRequested);
        break;
    default:
        EntityStateWrapper<LivingStateType>::WriteChangesToVector(changes, change);
        break;
    }
}

template <typename LivingStateType> requires std::is_base_of_v<LivingEntityState, LivingStateType>
LivingEntityStateWrapper<LivingStateType>::LivingEntityStateWrapper(uint16_t id, EEntityType entityType) : EntityStateWrapper<LivingStateType>(id, entityType)
{
}

template <typename LivingStateType> requires std::is_base_of_v<LivingEntityState, LivingStateType>
LivingEntityStateWrapper<LivingStateType>::LivingEntityStateWrapper(const LivingStateType& otherState) : EntityStateWrapper<LivingStateType>(otherState)
{
}

template <typename LivingStateType> requires std::is_base_of_v<LivingEntityState, LivingStateType>
void LivingEntityStateWrapper<LivingStateType>::SetHorizontalInput(float horizontalInput)
{
    this->State.HorizontalInput = horizontalInput;
    this->Changes.emplace(EChangeTypeEntity::HorizontalInput);
}

template <typename LivingStateType> requires std::is_base_of_v<LivingEntityState, LivingStateType>
void LivingEntityStateWrapper<LivingStateType>::SetVerticalInput(float verticalInput)
{
    this->State.VerticalInput = verticalInput;
    this->Changes.emplace(EChangeTypeEntity::VerticalInput);
}

template <typename LivingStateType> requires std::is_base_of_v<LivingEntityState, LivingStateType>
void LivingEntityStateWrapper<LivingStateType>::SetJumpRequested(bool jumpRequested)
{
    this->State.JumpRequested = jumpRequested;
    this->Changes.emplace(EChangeTypeEntity::JumpRequested);
}

template <typename LivingStateType> requires std::is_base_of_v<LivingEntityState, LivingStateType>
void LivingEntityStateWrapper<LivingStateType>::WriteChangesToVector(std::vector<uint8_t>& changes) const
{
    EntityStateWrapper<LivingStateType>::WriteChangesToVector(changes);
}

template <typename LivingStateType> requires std::is_base_of_v<LivingEntityState, LivingStateType>
void LivingEntityStateWrapper<LivingStateType>::ClearAllChanges()
{
    EntityStateWrapper<LivingStateType>::ClearAllChanges();
}
