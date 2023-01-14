#pragma once
#include "Entity.h"

class LivingEntity : public Entity
{
    void CalculateVelocity();

protected:
    bool JumpRequested;
    float HorizontalInput;
    float VerticalInput;
    LivingEntity(glm::vec3 entitySize, float x, float y, float z);
    void Tick() override;
};
