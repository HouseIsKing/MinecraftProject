#include "Entity.h"
#include "World/MultiPlayerWorld.h"

MultiPlayerWorld* Entity::World = nullptr;

Entity::Entity(const glm::vec3 entitySize, const float x, const float y, const float z) : IsGrounded(false), EntitySize(entitySize), EntityId(GetWorld()->RegisterEntity(this)), VelocityX(0), VelocityY(0), VelocityZ(0)
{
    EntityTransform.SetPosition(x + entitySize.x, y + entitySize.y, z + entitySize.z);
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

bool Entity::IsOnGround() const
{
    return IsGrounded;
}

void Entity::CheckCollisionAndMove()
{
    const float originalY = VelocityY;
    const glm::vec3 pos = GetTransform().GetPosition();
    auto myBoundingBox = BoundingBox(pos.x - EntitySize.x, pos.y - EntitySize.y, pos.z - EntitySize.z, pos.x + EntitySize.x, pos.y + EntitySize.y, pos.z + EntitySize.z);
    auto movementBox = BoundingBox(myBoundingBox);
    movementBox.Expand(VelocityX, VelocityY, VelocityZ);
    movementBox.Grow(1.0F, 1.0F, 1.0F);
    std::vector<BoundingBox> collidingBoxes = World->GetBlockBoxesInBoundingBox(movementBox);
    for (BoundingBox& box : collidingBoxes)
    {
        VelocityX = myBoundingBox.ClipCollisionX(box, VelocityX);
    }
    myBoundingBox.Move(VelocityX, 0.0F, 0.0F);
    GetTransform().Move(VelocityX, 0.0F, 0.0F);
    for (BoundingBox& box : collidingBoxes)
    {
        VelocityY = myBoundingBox.ClipCollisionY(box, VelocityY);
    }
    myBoundingBox.Move(0.0F, VelocityY, 0.0F);
    GetTransform().Move(0.0F, VelocityY, 0.0F);
    for (BoundingBox& box : collidingBoxes)
    {
        VelocityZ = myBoundingBox.ClipCollisionZ(box, VelocityZ);
    }
    GetTransform().Move(0.0F, 0.0F, VelocityZ);
    IsGrounded = originalY <= 0 && abs(VelocityY - originalY) > 0.001F;
}

void Entity::Tick()
{
}

void Entity::DoTick()
{
    Tick();
}

Transform& Entity::GetTransform()
{
    return EntityTransform;
}

BoundingBox Entity::GetBoundingBox()
{
    const glm::vec3 pos = GetTransform().GetPosition();
    return {pos.x - EntitySize.x, pos.y - EntitySize.y, pos.z - EntitySize.z, pos.x + EntitySize.x, pos.y + EntitySize.y, pos.z + EntitySize.z};
}

uint16_t Entity::GetEntityId() const
{
    return EntityId;
}

glm::vec3 Entity::GetEntitySize() const
{
    return EntitySize;
}

std::shared_ptr<Packet> Entity::GetTickPacket()
{
    auto packet = std::make_shared<Packet>(PacketHeader::ENTITY_DATA_PACKET);
    const glm::vec3 pos = GetTransform().GetPosition();
    const glm::vec3 rotation = GetTransform().GetRotation();
    *packet << GetEntityId() << pos.x << pos.y << pos.z << rotation.x << rotation.y << rotation.z;
    return packet;
}