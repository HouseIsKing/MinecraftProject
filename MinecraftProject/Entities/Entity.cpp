//
// Created by amit on 4/22/2022.
//

#include "Entity.h"
#include "../Util/EngineDefaults.h"
#include "../World/SinglePlayerWorld.h"

SinglePlayerWorld* Entity::World = nullptr;

Entity::Entity(uint16_t setEntityID, vec3 entitySize, float x, float y, float z) : IsGrounded(false), VelocityX(0), VelocityY(0), VelocityZ(0), EntityId(setEntityID), EntitySize(entitySize),
    TessellationHelper(EngineDefaults::GetShader(),x,y,z) {
}

Entity::~Entity()
{
}

void Entity::SetWorld(SinglePlayerWorld *newWorld) {
    Entity::World = newWorld;
}

SinglePlayerWorld *Entity::GetWorld() {
    return Entity::World;
}

bool Entity::IsOnGround() const {
    return IsGrounded;
}

void Entity::CheckCollisionAndMove() {
    float originalY = VelocityY;
    vec3 pos = TessellationHelper.GetTransform().GetPosition() + EntitySize;
    BoundingBox myBoundingBox = BoundingBox(pos.x-EntitySize.x, pos.y-EntitySize.y, pos.z-EntitySize.z, pos.x + EntitySize.x, pos.y + EntitySize.y, pos.z + EntitySize.z);
    auto movementBox = BoundingBox(myBoundingBox);
    movementBox.Expand(VelocityX, VelocityY, VelocityZ);
    movementBox.Grow(1.0f,1.0f,1.0f);
    std::vector<BoundingBox> collidingBoxes = World->GetBlockBoxesInBoundingBox(movementBox);
    for(BoundingBox& box : collidingBoxes)
    {
        VelocityX = myBoundingBox.ClipCollisionX(box,VelocityX);
    }
    myBoundingBox.Move(VelocityX,0,0);
    TessellationHelper.GetTransform().Move(VelocityX,0,0);
    for(BoundingBox& box : collidingBoxes)
    {
        VelocityY = myBoundingBox.ClipCollisionY(box,VelocityY);
    }
    myBoundingBox.Move(0,VelocityY,0);
    TessellationHelper.GetTransform().Move(0,VelocityY,0);
    for(BoundingBox& box : collidingBoxes)
    {
        VelocityZ = myBoundingBox.ClipCollisionZ(box,VelocityZ);
    }
    TessellationHelper.GetTransform().Move(0,0,VelocityZ);
    IsGrounded = originalY <= 0 && originalY != VelocityY;
}

void Entity::Render() {

}

void Entity::Tick() {
}

void Entity::DoTick() {
    Tick();
}

Transform& Entity::GetTransform()
{
    return TessellationHelper.GetTransform();
}
