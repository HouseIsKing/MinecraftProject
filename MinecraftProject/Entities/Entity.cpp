//
// Created by amit on 4/22/2022.
//

#include "Entity.h"
#include "../World/SinglePlayerWorld.h"

SinglePlayerWorld* Entity::world = nullptr;

Entity::Entity(uint16_t setEntityID, vec3 entitySize, float x, float y, float z) : isGrounded(false), velocityX(0), velocityY(0), velocityZ(0), entityID(setEntityID), entitySize(entitySize),
                                                                                tessellationHelper(EngineDefaults::getShader(),x,y,z) {
}

Entity::~Entity()
{
}

void Entity::setWorld(SinglePlayerWorld *newWorld) {
    Entity::world = newWorld;
}

SinglePlayerWorld *Entity::getWorld() {
    return Entity::world;
}

bool Entity::IsGrounded() const {
    return isGrounded;
}

void Entity::checkCollisionAndMove() {
    float originalY = velocityY;
    vec3 pos = tessellationHelper.GetTransform().getPosition() + entitySize;
    BoundingBox myBoundingBox = BoundingBox(pos.x-entitySize.x, pos.y-entitySize.y, pos.z-entitySize.z, pos.x + entitySize.x, pos.y + entitySize.y, pos.z + entitySize.z);
    auto movementBox = BoundingBox(myBoundingBox);
    movementBox.expand(velocityX, velocityY, velocityZ);
    movementBox.grow(1.0f,1.0f,1.0f);
    std::vector<BoundingBox> collidingBoxes = world->getBlockBoxesInBoundingBox(movementBox);
    for(BoundingBox& box : collidingBoxes)
    {
        velocityX = myBoundingBox.clipCollisionX(box,velocityX);
    }
    myBoundingBox.move(velocityX,0,0);
    tessellationHelper.GetTransform().move(velocityX,0,0);
    for(BoundingBox& box : collidingBoxes)
    {
        velocityY = myBoundingBox.clipCollisionY(box,velocityY);
    }
    myBoundingBox.move(0,velocityY,0);
    tessellationHelper.GetTransform().move(0,velocityY,0);
    for(BoundingBox& box : collidingBoxes)
    {
        velocityZ = myBoundingBox.clipCollisionZ(box,velocityZ);
    }
    tessellationHelper.GetTransform().move(0,0,velocityZ);
    isGrounded = originalY <= 0 && originalY != velocityY;
}

void Entity::Render() {

}

void Entity::tick() {
}

void Entity::doTick() {
    tick();
}

Transform& Entity::getTransform()
{
    return tessellationHelper.GetTransform();
}
