#pragma once
#include "Entity.h"

template <typename T>
class LivingEntity : public Entity<T>
{
    void CalculateVelocity();

protected:
    bool JumpRequested;
    float HorizontalInput;
    float VerticalInput;
    LivingEntity(glm::vec3 entitySize, float x, float y, float z);
    void Tick() override;
};

template <typename T>
LivingEntity<T>::LivingEntity(const glm::vec3 entitySize, const float x, const float y, const float z) : Entity<T>(entitySize,
    x, y, z), JumpRequested(false), HorizontalInput(0), VerticalInput(0)
{
}

template <typename T>
void LivingEntity<T>::CalculateVelocity()
{
    float speedModifier = 0.1F;
    if (JumpRequested && this->IsOnGround())
    {
        this->VelocityY = 0.5F;
    }
    if (!this->IsOnGround())
    {
        speedModifier *= 0.2F;
    }
    this->VelocityY -= 0.08F;
    const glm::vec3 rotation = this->GetTransform().GetRotation();
    if (const float d = sqrt(HorizontalInput * HorizontalInput + VerticalInput * VerticalInput); d > 0.001F)
    {
        speedModifier /= d;
        this->VelocityX += speedModifier * (VerticalInput * cos(glm::radians(rotation.y)) - HorizontalInput * sin(glm::radians(rotation.y)));
        this->VelocityZ += speedModifier * (HorizontalInput * cos(glm::radians(rotation.y)) + VerticalInput * sin(glm::radians(rotation.y)));
    }
    this->CheckCollisionAndMove();
    this->VelocityX *= 0.91F;
    this->VelocityZ *= 0.91F;
    this->VelocityY *= 0.98F;
    if (this->IsOnGround())
    {
        this->VelocityX *= 0.7F;
        this->VelocityZ *= 0.7F;
    }
}

template <typename T>
void LivingEntity<T>::Tick()
{
    Entity<T>::Tick();
    CalculateVelocity();
}
