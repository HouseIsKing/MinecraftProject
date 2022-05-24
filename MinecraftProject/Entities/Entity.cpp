//
// Created by amit on 4/22/2022.
//

#include "Entity.h"
#include "../Util/EngineDefaults.h"
#include "../World/SinglePlayerWorld.h"

SinglePlayerWorld* Entity::World = nullptr;

Entity::Entity(uint16_t entityId, vec3 entitySize, float x, float y, float z) : IsGrounded(false), EntitySize(entitySize), EntityId(entityId), VelocityX(0), VelocityY(0), VelocityZ(0),
    Tessellation(EngineDefaults::GetShader(), x + entitySize.x, y + entitySize.y, z + entitySize.z)
{
}

Entity::~Entity() = default;

void Entity::SetWorld(SinglePlayerWorld* newWorld)
{
    World = newWorld;
}

SinglePlayerWorld* Entity::GetWorld()
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
    const vec3 pos = Tessellation.GetTransform().GetPosition();
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
    Tessellation.GetTransform().Move(VelocityX, 0.0F, 0.0F);
    for (BoundingBox& box : collidingBoxes)
    {
        VelocityY = myBoundingBox.ClipCollisionY(box, VelocityY);
    }
    myBoundingBox.Move(0.0F, VelocityY, 0.0F);
    Tessellation.GetTransform().Move(0.0F, VelocityY, 0.0F);
    for (BoundingBox& box : collidingBoxes)
    {
        VelocityZ = myBoundingBox.ClipCollisionZ(box, VelocityZ);
    }
    Tessellation.GetTransform().Move(0.0F, 0.0F, VelocityZ);
    IsGrounded = originalY <= 0 && abs(VelocityY - originalY) > 0.001F;
}

void Entity::Render()
{
}

void Entity::Tick()
{
}

void Entity::DoTick()
{
    Tick();
}

void Entity::DoRender()
{
    Render();
}

Transform& Entity::GetTransform()
{
    return Tessellation.GetTransform();
}
