#include <iostream>

#include "World/MultiPlayerWorld.h"
#include <GLFW/glfw3.h>

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
    MultiPlayerWorld world{256, 64, 256};
    float ticksTimer = 0;
    auto start = static_cast<float>(glfwGetTime());
    while (run)
    {
        int i;
        for (i = 0; i < static_cast<int>(ticksTimer / EngineDefaults::TICK_RATE); i++)
        {
            world.Tick();
        }
        world.Run();
        const auto end = static_cast<float>(glfwGetTime());
        ticksTimer -= static_cast<float>(i) * EngineDefaults::TICK_RATE;
        ticksTimer += end - start;
        if (end - start > 0.01F)
        {
            std::cout << "Frame time: " << end - start << std::endl;
        }
        start = end;
    }
    return 0;
}
