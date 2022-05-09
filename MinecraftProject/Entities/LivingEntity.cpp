//
// Created by amit on 4/22/2022.
//

#include "LivingEntity.h"

LivingEntity::LivingEntity(uint16_t entityId, vec3 entitySize, float x, float y, float z) : Entity(entityId, entitySize,
                                                                                                   x, y, z) , JumpRequested(false) , HorizontalInput(0) , VerticalInput(0) {
}

void LivingEntity::CalculateVelocity() {
    float speedModifier = 0.02f;
    if(JumpRequested && IsOnGround()) {
        VelocityY = 0.12f;
    }
    JumpRequested = false;
    if(!IsOnGround()) {
        speedModifier *= 0.25f;
    }
    VelocityY -= 0.005f;
    vec3 rotation = TessellationHelper.GetTransform().GetRotation();
    if (VerticalInput != 0 || HorizontalInput != 0)
    {
		speedModifier /= sqrt(HorizontalInput * HorizontalInput + VerticalInput * VerticalInput);
        VelocityX += speedModifier * (VerticalInput * cos(radians(rotation.y)) - HorizontalInput * sin(radians(rotation.y)));
        VelocityZ += speedModifier * (HorizontalInput * cos(radians(rotation.y)) + VerticalInput * sin(radians(rotation.y)));
    }
    VelocityX *= 0.91f;
    VelocityZ *= 0.91f;
    VelocityY *= 0.98f;
    if(IsOnGround()) {
        VelocityX *= 0.8f;
        VelocityZ *= 0.8f;
    }
    CheckCollisionAndMove();
}

void LivingEntity::Tick() {
    CalculateVelocity();
    Entity::Tick();
}
