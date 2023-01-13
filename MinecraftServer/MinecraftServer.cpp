#include <iostream>

#include "Network/ServerNetworkManager.h"
#include "Util/ThreadSafeSet.h"

bool run = true;

BOOL APIENTRY CtrlHandler(const DWORD fdwCtrlType)
{
    if (fdwCtrlType == CTRL_CLOSE_EVENT)
    {
        run = false;
        return TRUE;
    }
    return FALSE;
}

int main(int /*argc*/, char* /*argv*/[])
{
    SetConsoleCtrlHandler(CtrlHandler, TRUE);
    ServerNetworkManager networkManager;
    networkManager.Start();
    std::unordered_set<std::shared_ptr<ConnectionToClientInterface>, ConnectionHasher, ConnectionEqual> connections;
    while (run)
    {
        std::shared_ptr<ConnectionToClient> newCon = networkManager.GetNextNewConnection();
        while (newCon != nullptr)
        {
            connections.insert(newCon);
            newCon = networkManager.GetNextNewConnection();
        }
        std::cout << "Number of connections: " << connections.size() << std::endl;
        if (!connections.empty())
        {
            for (const auto& con : connections)
            {
                std::cout << con->GetPlayerName() << std::endl;
            }
        }
    }
    return 0;
}
