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
    vec3 pos = tessellationHelper.getTransform().getPosition();
    BoundingBox myBoundingBox = BoundingBox(pos.x-entitySize.x, pos.y-entitySize.y, pos.z-entitySize.z, pos.x + entitySize.x, pos.y + entitySize.y, pos.z + entitySize.z);
    auto movementBox = BoundingBox(myBoundingBox);
    movementBox.expand(velocityX, velocityY, velocityZ);
    movementBox.grow(1.0f,1.0f,1.0f);
    std::vector<Block*> collidingBoxes = world->getBlocksInBoundingBox(movementBox);
    for(Block* box : collidingBoxes)
    {
        velocityX = myBoundingBox.clipCollisionX(box->getBoundingBox(),velocityX);
    }
    myBoundingBox.move(velocityX,0,0);
    tessellationHelper.getTransform().move(velocityX,0,0);
    for(Block* box : collidingBoxes)
    {
        velocityY = myBoundingBox.clipCollisionY(box->getBoundingBox(),velocityY);
    }
    myBoundingBox.move(0,velocityY,0);
    tessellationHelper.getTransform().move(0,velocityY,0);
    for(Block* box : collidingBoxes)
    {
        velocityZ = myBoundingBox.clipCollisionZ(box->getBoundingBox(),velocityZ);
    }
    tessellationHelper.getTransform().move(0,0,velocityZ);
    isGrounded = originalY <= 0 && originalY != velocityY;
}

void Entity::Render() {

}

void Entity::tick() {
}

void Entity::doTick() {
    tick();
}
