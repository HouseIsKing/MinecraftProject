#pragma once
#include "Util/EngineDefaults.h"


struct EntityState : State
{
    TransformStruct EntityTransform{};
    glm::vec3 EntityVelocity{};
    bool IsGrounded{false};
    uint16_t EntityId{};
    EEntityType EntityType{EEntityType::Player};
};
