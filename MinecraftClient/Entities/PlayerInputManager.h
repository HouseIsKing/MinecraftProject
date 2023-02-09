#pragma once
#include "Network/ClientNetworkManager.h"

struct PlayerInputManager
{
    float PrevMouseX{0.0F};
    float PrevMouseY{0.0F};
    ClientInputStruct Input{};
};
