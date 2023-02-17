#include "ServerManager.h"
#include <GLFW/glfw3.h>
#include <World/World.h>
#include <iostream>

bool run = true;

BOOL APIENTRY CtrlHandler(const DWORD fdwCtrlType)
{
    if (fdwCtrlType == CTRL_CLOSE_EVENT || fdwCtrlType == CTRL_C_EVENT)
    {
        run = false;
        return TRUE;
    }
    return FALSE;
}

int main(int /*argc*/, char* /*argv*/[])
{
    glfwInit();
    SetConsoleCtrlHandler(CtrlHandler, TRUE);
    ServerManager world(256, 64, 256);
    world.NewTick();
    float ticksTimer = 0;
    auto start = static_cast<float>(glfwGetTime());
    while (run)
    {
        int i;
        for (i = 0; i < static_cast<int>(ticksTimer / EngineDefaults::TICK_RATE); i++)
        {
            world.NewTick();
        }
        world.Run();
        const auto end = static_cast<float>(glfwGetTime());
        ticksTimer -= static_cast<float>(i) * EngineDefaults::TICK_RATE;
        ticksTimer += end - start;
        start = end;
    }
    return 0;
}
