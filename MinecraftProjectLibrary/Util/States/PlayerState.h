#pragma once
#include "LivingEntityState.h"

struct PlayerState final : LivingEntityState
{
    ClientInputStruct InputState;
    float CameraPitch{0.0F};
};
