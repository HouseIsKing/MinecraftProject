#include "World/MultiPlayerWorld.h"

bool run = true;
constexpr float TICK_RATE = 1.0F / 20.0F;

BOOL APIENTRY CtrlHandler(const DWORD fdwCtrlType)
{
    if (fdwCtrlType == CTRL_CLOSE_EVENT || fdwCtrlType == CTRL_C_EVENT)
    {
        run = false;
    }
    return FALSE;
}

int main(int /*argc*/, char* /*argv*/[])
{
    SetConsoleCtrlHandler(CtrlHandler, TRUE);
    MultiPlayerWorld world{256, 64, 256};
    float ticksTimer = 0;
    while (run)
    {
        std::chrono::time_point<std::chrono::system_clock> start = std::chrono::system_clock::now();
        int i;
        for (i = 0; i < static_cast<int>(ticksTimer / TICK_RATE); i++)
        {
            world.Tick();
        }
        world.Run();
        std::chrono::time_point<std::chrono::system_clock> end = std::chrono::system_clock::now();
        ticksTimer -= static_cast<float>(i) * TICK_RATE;
        ticksTimer += std::chrono::duration<float>(end - start).count();
    }
    return 0;
}
