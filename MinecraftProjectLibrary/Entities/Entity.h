#pragma once
#include "Util/BoundingBox.h"
#include "Util/States/EntityStateWrapper.h"

class World;

template <typename Wrapper, typename StateType> requires std::is_base_of_v<EntityStateWrapper<StateType>, Wrapper>
class Entity
{
protected:
    Wrapper State;
    void CheckCollisionAndMove();
    virtual void ApplyRevertEntityChange(const std::vector<uint8_t>& changes, size_t& pos, EChangeTypeEntity change, bool revert = true);

public:
    Entity(const glm::vec3& entitySize, const glm::vec3& position, const uint16_t& id);
    explicit Entity(const StateType& state);
    virtual ~Entity() = default;
    Entity(const Entity&) = delete;
    Entity& operator=(const Entity&) = delete;
    Entity& operator=(Entity&&) = delete;
    Entity(Entity&&) = delete;
    virtual void Tick();
    [[nodiscard]] BoundingBox GetBoundingBox() const;
    [[nodiscard]] uint16_t GetEntityId() const;
    [[nodiscard]] EEntityType GetEntityType() const;
    [[nodiscard]] const StateType& GetState() const;
    [[nodiscard]] const StateType& GetOldState() const;
    void AttachEntityChange(std::vector<uint8_t>& changes) const;
    void AttachEntityData(std::vector<uint8_t>& data) const;
    void RevertEntityChanges(const std::vector<uint8_t>& changes, size_t& pos);
    void ClearAllChanges();
};

template <typename Wrapper, typename StateType> requires std::is_base_of_v<EntityStateWrapper<StateType>, Wrapper>
void Entity<Wrapper, StateType>::CheckCollisionAndMove()
{
    const StateType& state = State.GetState();
    const float originalY = state.EntityVelocity.y;
    auto myBoundingBox = GetBoundingBox();
    auto movementBox = BoundingBox(myBoundingBox);
    movementBox.Expand(state.EntityVelocity.x, state.EntityVelocity.y, state.EntityVelocity.z);
    movementBox.Grow(1.0F, 1.0F, 1.0F);
    std::vector<BoundingBox> collidingBoxes = World::GetWorld()->GetBlockBoxesInBoundingBox(movementBox);
    glm::vec3 entityPos = state.EntityTransform.Position;
    glm::vec3 entityVelocity = state.EntityVelocity;
    for (BoundingBox& box : collidingBoxes)
    {
        entityVelocity.x = myBoundingBox.ClipCollisionX(box, entityVelocity.x);
    }
    myBoundingBox.Move(entityVelocity.x, 0.0F, 0.0F);
    entityPos.x += entityVelocity.x;
    for (BoundingBox& box : collidingBoxes)
    {
        entityVelocity.y = myBoundingBox.ClipCollisionY(box, entityVelocity.y);
    }
    myBoundingBox.Move(0.0F, entityVelocity.y, 0.0F);
    entityPos.y += entityVelocity.y;
    for (BoundingBox& box : collidingBoxes)
    {
        entityVelocity.z = myBoundingBox.ClipCollisionZ(box, entityVelocity.z);
    }
    entityPos.z += entityVelocity.z;
    State.SetPosition(entityPos);
    State.SetVelocity(entityVelocity);
    State.SetIsGrounded(originalY <= 0 && abs(entityVelocity.y - originalY) > 0.001F);
}

template <typename Wrapper, typename StateType> requires std::is_base_of_v<EntityStateWrapper<StateType>, Wrapper>
void Entity<Wrapper, StateType>::ApplyRevertEntityChange(const std::vector<uint8_t>& changes, size_t& pos,
                                                         const EChangeTypeEntity change, bool revert)
{
    switch (change)
    {
    case EChangeTypeEntity::Position:
        {
            if (revert)
            {
                State.SetPosition(EngineDefaults::ReadDataFromVector<glm::vec3>(changes, pos));
                pos += sizeof(glm::vec3);
            }
            else
            {
                pos += sizeof(glm::vec3);
                State.SetPosition(EngineDefaults::ReadDataFromVector<glm::vec3>(changes, pos));
            }
            break;
        }
    case EChangeTypeEntity::Rotation:
        {
            if (revert)
            {
                State.SetRotation(EngineDefaults::ReadDataFromVector<glm::vec3>(changes, pos));
                pos += sizeof(glm::vec3);
            }
            else
            {
                pos += sizeof(glm::vec3);
                State.SetRotation(EngineDefaults::ReadDataFromVector<glm::vec3>(changes, pos));
            }
            break;
        }
    case EChangeTypeEntity::Scale:
        {
            if (revert)
            {
                State.SetScale(EngineDefaults::ReadDataFromVector<glm::vec3>(changes, pos));
                pos += sizeof(glm::vec3);
            }
            else
            {
                pos += sizeof(glm::vec3);
                State.SetScale(EngineDefaults::ReadDataFromVector<glm::vec3>(changes, pos));
            }
            break;
        }
    case EChangeTypeEntity::Velocity:
        {
            if (revert)
            {
                State.SetVelocity(EngineDefaults::ReadDataFromVector<glm::vec3>(changes, pos));
                pos += sizeof(glm::vec3);
            }
            else
            {
                pos += sizeof(glm::vec3);
                State.SetVelocity(EngineDefaults::ReadDataFromVector<glm::vec3>(changes, pos));
            }
            break;
        }
    case EChangeTypeEntity::IsGrounded:
        {
            if (revert)
            {
                State.SetIsGrounded(EngineDefaults::ReadDataFromVector<bool>(changes, pos));
                pos += sizeof(bool);
            }
            else
            {
                pos += sizeof(bool);
                State.SetIsGrounded(EngineDefaults::ReadDataFromVector<bool>(changes, pos));
            }
            break;
        }
    default:
        break;
    }
}

template <typename Wrapper, typename StateType> requires std::is_base_of_v<EntityStateWrapper<StateType>, Wrapper>
Entity<Wrapper, StateType>::Entity(const glm::vec3& entitySize, const glm::vec3& position, const uint16_t& id) : State(id)
{
    State.SetPosition(position);
    State.SetScale(entitySize);
    State.ClearAllChanges();
}

template <typename Wrapper, typename StateType> requires std::is_base_of_v<EntityStateWrapper<StateType>, Wrapper>
Entity<Wrapper, StateType>::Entity(const StateType& state) : State(state)
{
}

template <typename Wrapper, typename StateType> requires std::is_base_of_v<EntityStateWrapper<StateType>, Wrapper>
void Entity<Wrapper, StateType>::Tick()
{
}

template <typename Wrapper, typename StateType> requires std::is_base_of_v<EntityStateWrapper<StateType>, Wrapper>
BoundingBox Entity<Wrapper, StateType>::GetBoundingBox() const
{
    const StateType& state = State.GetState();
    const glm::vec3& pos = state.EntityTransform.Position;
    const glm::vec3& scale = state.EntityTransform.Scale;
    return {pos.x - scale.x, pos.y - scale.y, pos.z - scale.z, pos.x + scale.x, pos.y + scale.y, pos.z + scale.z};
}

template <typename Wrapper, typename StateType> requires std::is_base_of_v<EntityStateWrapper<StateType>, Wrapper>
uint16_t Entity<Wrapper, StateType>::GetEntityId() const
{
    return State.GetState().EntityId;
}

template <typename Wrapper, typename StateType> requires std::is_base_of_v<EntityStateWrapper<StateType>, Wrapper>
EEntityType Entity<Wrapper, StateType>::GetEntityType() const
{
    return State.GetState().EntityType;
}

template <typename Wrapper, typename StateType> requires std::is_base_of_v<EntityStateWrapper<StateType>, Wrapper>
const StateType& Entity<Wrapper, StateType>::GetState() const
{
    return State.GetState();
}

template <typename Wrapper, typename StateType> requires std::is_base_of_v<EntityStateWrapper<StateType>, Wrapper>
const StateType& Entity<Wrapper, StateType>::GetOldState() const
{
    return State.GetOldState();
}

template <typename Wrapper, typename StateType> requires std::is_base_of_v<EntityStateWrapper<StateType>, Wrapper>
void Entity<Wrapper, StateType>::AttachEntityChange(std::vector<uint8_t>& changes) const
{
    State.WriteChangesToVector(changes);
}

template <typename Wrapper, typename StateType> requires std::is_base_of_v<EntityStateWrapper<StateType>, Wrapper>
void Entity<Wrapper, StateType>::AttachEntityData(std::vector<uint8_t>& data) const
{
    EngineDefaults::EmplaceReplaceDataInVector(data, &State.GetState());
}

template <typename Wrapper, typename StateType> requires std::is_base_of_v<EntityStateWrapper<StateType>, Wrapper>
void Entity<Wrapper, StateType>::RevertEntityChanges(const std::vector<uint8_t>& changes, size_t& pos)
{
    const uint8_t changeCount = EngineDefaults::ReadDataFromVector<uint8_t>(changes, pos);
    for (int i = 0; i < changeCount; i++)
    {
        const EChangeTypeEntity change = EngineDefaults::ReadDataFromVector<EChangeTypeEntity>(changes, pos);
        ApplyRevertEntityChange(changes, pos, change);
    }
}

template <typename Wrapper, typename StateType> requires std::is_base_of_v<EntityStateWrapper<StateType>, Wrapper>
void Entity<Wrapper, StateType>::ClearAllChanges()
{
    State.ClearAllChanges();
}
