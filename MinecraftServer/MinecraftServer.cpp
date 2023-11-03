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
    while (run)
    {
        world.Run();
    }
    return 0;
}
