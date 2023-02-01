#include "Entity.h"
#include "World/MultiPlayerWorld.h"

MultiPlayerWorld* Entity::World = nullptr;

Entity::Entity(const glm::vec3 entitySize, const float x, const float y, const float z, EntityState* state) : State(state)
{
    State->EntityTransform.Scale = entitySize;
    State->EntityTransform.Position = glm::vec3(x, y, z);
}

Entity::~Entity() = default;

void Entity::SetWorld(MultiPlayerWorld* newWorld)
{
    World = newWorld;
}

MultiPlayerWorld* Entity::GetWorld()
{
    return World;
}

void Entity::CheckCollisionAndMove() const
{
    const float originalY = State->EntityVelocity.y;
    auto myBoundingBox = GetBoundingBox();
    auto movementBox = BoundingBox(myBoundingBox);
    movementBox.Expand(State->EntityVelocity.x, State->EntityVelocity.y, State->EntityVelocity.z);
    movementBox.Grow(1.0F, 1.0F, 1.0F);
    std::vector<BoundingBox> collidingBoxes = World->GetBlockBoxesInBoundingBox(movementBox);
    for (BoundingBox& box : collidingBoxes)
    {
        State->EntityVelocity.x = myBoundingBox.ClipCollisionX(box, State->EntityVelocity.x);
    }
    myBoundingBox.Move(State->EntityVelocity.x, 0.0F, 0.0F);
    State->EntityTransform.Position.x += State->EntityVelocity.x;
    for (BoundingBox& box : collidingBoxes)
    {
        State->EntityVelocity.y = myBoundingBox.ClipCollisionY(box, State->EntityVelocity.y);
    }
    myBoundingBox.Move(0.0F, State->EntityVelocity.y, 0.0F);
    State->EntityTransform.Position.y += State->EntityVelocity.y;
    for (BoundingBox& box : collidingBoxes)
    {
        State->EntityVelocity.z = myBoundingBox.ClipCollisionZ(box, State->EntityVelocity.z);
    }
    State->EntityTransform.Position.z += State->EntityVelocity.z;
    State->IsGrounded = originalY <= 0 && abs(State->EntityVelocity.y - originalY) > 0.001F;
}

void Entity::Tick()
{
}

TransformStruct& Entity::GetTransform() const
{
    return State->EntityTransform;
}

BoundingBox Entity::GetBoundingBox() const
{
    const glm::vec3& pos = State->EntityTransform.Position;
    const glm::vec3& scale = State->EntityTransform.Scale;
    return {pos.x - scale.x, pos.y - scale.y, pos.z - scale.z, pos.x + scale.x, pos.y + scale.y, pos.z + scale.z};
}

uint16_t Entity::GetEntityId() const
{
    return State->EntityId;
}

EEntityType Entity::GetEntityType() const
{
    return State->EntityType;
}

EntityState* Entity::GetEntityState() const
{
    return new EntityState(*State);
}

void Entity::ApplyEntityChanges(const std::vector<uint8_t>& changes, size_t& pos)
{
    const uint8_t changesCount = changes[pos];
    pos += sizeof(uint8_t);
    for (int i = 0; i < changesCount; i++)
    {
        const EChangeTypeEntity changeTypeEntity = *reinterpret_cast<const EChangeTypeEntity*>(&changes[pos]);
        pos += sizeof(EChangeTypeEntity);
        ApplyEntityChange(changes, pos, changeTypeEntity);
    }
}

void Entity::ApplyEntityChange(const std::vector<uint8_t>& changes, size_t& pos, const EChangeTypeEntity change)
{
    switch (change)
    {
    case EChangeTypeEntity::Position:
        {
            State->EntityTransform.Position = *reinterpret_cast<const glm::vec3*>(&changes[pos]);
            pos += sizeof(glm::vec3);
            break;
        }
    case EChangeTypeEntity::Rotation:
        {
            State->EntityTransform.Rotation = *reinterpret_cast<const glm::vec3*>(&changes[pos]);
            pos += sizeof(glm::vec3);
            break;
        }
    case EChangeTypeEntity::Scale:
        {
            State->EntityTransform.Scale = *reinterpret_cast<const glm::vec3*>(&changes[pos]);
            pos += sizeof(glm::vec3);
            break;
        }
    case EChangeTypeEntity::Velocity:
        {
            State->EntityVelocity = *reinterpret_cast<const glm::vec3*>(&changes[pos]);
            pos += sizeof(glm::vec3);
            break;
        }
    case EChangeTypeEntity::IsGrounded:
        {
            State->IsGrounded = *reinterpret_cast<const bool*>(&changes[pos]);
            pos += sizeof(bool);
            break;
        }
    default:
        break;
    }
}
