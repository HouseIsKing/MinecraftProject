#include "ServerNetworkManager.h"

#include <iostream>

void ServerNetworkManager::RunAcceptor()
{
    NewConnection = std::make_shared<ConnectionToClient>(Context, this);
    Acceptor.async_accept(NewConnection->GetSocket(), [this](const asio::error_code& error)
    {
        std::cout << error.message() << std::endl;
        NewConnection->Start();
        NewConnections.Push(NewConnection);
        RunAcceptor();
    });
}

ServerNetworkManager::ServerNetworkManager() : Acceptor(Context, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), 25565))
{
}

ServerNetworkManager::~ServerNetworkManager()
{
    Context.stop();
    ContextThread.join();
}

void ServerNetworkManager::AddPacket(const std::shared_ptr<PacketData>& packet)
{
    Packets.Push(packet);
}

void ServerNetworkManager::Start()
{
    RunAcceptor();
}

std::shared_ptr<PacketData> ServerNetworkManager::GetNextPacket()
{
    if (Packets.GetSize() > 0)
    {
        return Packets.Pop();
    }
    return nullptr;
}

std::shared_ptr<ConnectionToClient> ServerNetworkManager::GetNextNewConnection()
{
    if (NewConnections.GetSize() > 0)
    {
        return NewConnections.Pop();
    }
    return nullptr;
}

std::shared_ptr<ConnectionToClient> ServerNetworkManager::GetNextRemovedConnection()
{
    if (RemovedConnections.GetSize() > 0)
    {
        return RemovedConnections.Pop();
    }
    return nullptr;
}
