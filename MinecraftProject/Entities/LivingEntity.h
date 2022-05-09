//
// Created by amit on 4/22/2022.
//

#pragma once
#include "Entity.h"

using glm::sin;
using glm::cos;
using glm::radians;

class LivingEntity : public Entity
{
    void CalculateVelocity();
protected:
    bool JumpRequested;
    float HorizontalInput;
    float VerticalInput;
    LivingEntity(uint16_t entityId, vec3 entitySize, float x, float y, float z);
    void Tick() override;
};
