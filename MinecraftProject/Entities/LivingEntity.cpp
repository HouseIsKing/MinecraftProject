//
// Created by amit on 4/22/2022.
//

#include "LivingEntity.h"

LivingEntity::LivingEntity(const vec3 entitySize, const float x, const float y, const float z) : Entity(entitySize,
    x, y, z), JumpRequested(false), HorizontalInput(0), VerticalInput(0)
{
}

void LivingEntity::CalculateVelocity()
{
    float speedModifier = 0.1F;
    if (JumpRequested && IsOnGround())
    {
        VelocityY = 0.5F;
    }
    if (!IsOnGround())
    {
        speedModifier *= 0.2F;
    }
    VelocityY -= 0.08F;
    const vec3 rotation = GetTransform().GetRotation();
    if (const float d = sqrt(HorizontalInput * HorizontalInput + VerticalInput * VerticalInput); d > 0.001F)
    {
        speedModifier /= d;
        VelocityX += speedModifier * (VerticalInput * cos(radians(rotation.y)) - HorizontalInput * sin(radians(rotation.y)));
        VelocityZ += speedModifier * (HorizontalInput * cos(radians(rotation.y)) + VerticalInput * sin(radians(rotation.y)));
    }
    CheckCollisionAndMove();
    VelocityX *= 0.91F;
    VelocityZ *= 0.91F;
    VelocityY *= 0.98F;
    if (IsOnGround())
    {
        VelocityX *= 0.7F;
        VelocityZ *= 0.7F;
    }
}

void LivingEntity::Tick()
{
    Entity::Tick();
    CalculateVelocity();
}
