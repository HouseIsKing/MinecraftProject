#include "Entity.h"

#include "World/Generic/World.h"

World* Entity::TheWorld = nullptr;

Entity::Entity(const glm::vec3 entitySize, const float x, const float y, const float z, EntityState* state) : State(state), Tessellation(&State->EntityTransform), PrevTransform(State->EntityTransform)
{
    State->EntityTransform.Scale = entitySize;
    State->EntityTransform.Position = glm::vec3(x, y, z);
}

void Entity::CheckCollisionAndMove() const
{
    const float originalY = State->EntityVelocity.y;
    auto myBoundingBox = GetBoundingBox();
    auto movementBox = BoundingBox(myBoundingBox);
    movementBox.Expand(State->EntityVelocity.x, State->EntityVelocity.y, State->EntityVelocity.z);
    movementBox.Grow(1.0F, 1.0F, 1.0F);
    std::vector<BoundingBox> collidingBoxes = TheWorld->GetBlockBoxesInBoundingBox(movementBox);
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

void Entity::SetWorld(World* newWorld)
{
    TheWorld = newWorld;
}

void Entity::ApplyEntityChange(const std::vector<uint8_t>& changes, size_t& pos, EChangeTypeEntity change)
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

void Entity::Render(float /*partialTick*/)
{
}

void Entity::Tick()
{
    PrevTransform = GetTransform();
}

TransformStruct& Entity::GetTransform() const
{
    return State->EntityTransform;
}

BoundingBox Entity::GetBoundingBox() const
{
    return {State->EntityTransform.Position.x - State->EntityTransform.Scale.x, State->EntityTransform.Position.y - State->EntityTransform.Scale.y, State->EntityTransform.Position.z - State->EntityTransform.Scale.z, State->EntityTransform.Position.x + State->EntityTransform.Scale.x, State->EntityTransform.Position.y + State->EntityTransform.Scale.y, State->EntityTransform.Position.z + State->EntityTransform.Scale.z};
}

uint16_t Entity::GetEntityId() const
{
    return State->EntityId;
}

glm::vec3 Entity::GetEntitySize() const
{
    return State->EntityTransform.Scale;
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

