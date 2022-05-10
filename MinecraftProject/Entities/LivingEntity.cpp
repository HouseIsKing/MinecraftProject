//
// Created by amit on 4/22/2022.
//

#include "LivingEntity.h"

LivingEntity::LivingEntity(const uint16_t entityId, const vec3 entitySize, const float x, const float y, const float z) : Entity(entityId, entitySize,
    x, y, z), JumpRequested(false), HorizontalInput(0), VerticalInput(0)
{
}

void LivingEntity::CalculateVelocity()
{
    float speedModifier = 0.02F;
    if (JumpRequested && IsOnGround())
    {
        VelocityY = 0.12F;
    }
    JumpRequested = false;
    if (!IsOnGround())
    {
        speedModifier *= 0.25F;
    }
    VelocityY -= 0.005F;
    const vec3 rotation = TessellationHelper.GetTransform().GetRotation();
    if (const float d = sqrt(HorizontalInput * HorizontalInput + VerticalInput * VerticalInput); d > 0.001F)
    {
        speedModifier /= d;
        VelocityX += speedModifier * (VerticalInput * cos(radians(rotation.y)) - HorizontalInput * sin(radians(rotation.y)));
        VelocityZ += speedModifier * (HorizontalInput * cos(radians(rotation.y)) + VerticalInput * sin(radians(rotation.y)));
    }
    VelocityX *= 0.91F;
    VelocityZ *= 0.91F;
    VelocityY *= 0.98F;
    if (IsOnGround())
    {
        VelocityX *= 0.8F;
        VelocityZ *= 0.8F;
    }
    CheckCollisionAndMove();
}

void LivingEntity::Tick()
{
    CalculateVelocity();
    Entity::Tick();
}
