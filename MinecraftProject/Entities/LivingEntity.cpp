//
// Created by amit on 4/22/2022.
//

#include "LivingEntity.h"

LivingEntity::LivingEntity(uint16_t entityID, vec3 entitySize, float x, float y, float z) : Entity(entityID, entitySize,
                                                                                                   x, y, z) , jumpRequested(false) , horizontalInput(0) , verticalInput(0) {
}

void LivingEntity::calculateVelocity() {
    float speedModifier = 0.02f;
    if(jumpRequested && IsGrounded()) {
        velocityY = 0.12f;
    }
    jumpRequested = false;
    if(!IsGrounded()) {
        speedModifier *= 0.25f;
    }
    velocityY -= 0.005f;
    vec3 rotation = tessellationHelper.GetTransform().getRotation();
    if (verticalInput != 0 || horizontalInput != 0)
    {
		speedModifier /= sqrt(horizontalInput * horizontalInput + verticalInput * verticalInput);
        velocityX += speedModifier * (verticalInput * cos(radians(rotation.y)) - horizontalInput * sin(radians(rotation.y)));
        velocityZ += speedModifier * (horizontalInput * cos(radians(rotation.y)) + verticalInput * sin(radians(rotation.y)));
    }
    velocityX *= 0.91f;
    velocityZ *= 0.91f;
    velocityY *= 0.98f;
    if(IsGrounded()) {
        velocityX *= 0.8f;
        velocityZ *= 0.8f;
    }
    checkCollisionAndMove();
}

void LivingEntity::tick() {
    calculateVelocity();
    Entity::tick();
}
