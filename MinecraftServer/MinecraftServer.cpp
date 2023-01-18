#include "World/MultiPlayerWorld.h"
#include <GLFW/glfw3.h>

bool run = true;
constexpr float TICK_RATE = 0.05F;

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
        world.PartialTick = ticksTimer - TICK_RATE * static_cast<float>(static_cast<int>(ticksTimer / TICK_RATE));
        for (i = 0; i < static_cast<int>(ticksTimer / TICK_RATE); i++)
        {
            world.Tick();
        }
        world.Run();
        const auto end = static_cast<float>(glfwGetTime());
        ticksTimer -= static_cast<float>(i) * TICK_RATE;
        ticksTimer += end - start;
        start = end;
    }
    return 0;
}
