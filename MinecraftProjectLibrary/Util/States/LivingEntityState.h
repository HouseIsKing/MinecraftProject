#pragma once
#include "EntityState.h"

struct LivingEntityState : EntityState
{
    bool JumpRequested{false};
    float HorizontalInput{0.0F};
    float VerticalInput{0.0F};
};