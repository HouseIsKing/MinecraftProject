#include "ClientNetworkManager.h"

#include <iostream>

void ClientNetworkManager::RunAcceptor()
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

ClientNetworkManager::ClientNetworkManager() : Acceptor(Context, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), 25565))
{
}

ClientNetworkManager::~ClientNetworkManager()
{
    Context.stop();
    ContextThread.join();
}

void ClientNetworkManager::AddPacket(const std::shared_ptr<PacketData>& packet)
{
    Packets.Push(packet);
}

void ClientNetworkManager::Start()
{
    RunAcceptor();
}

std::shared_ptr<PacketData> ClientNetworkManager::GetNextPacket()
{
    if (Packets.GetSize() > 0)
    {
        return Packets.Pop();
    }
    return nullptr;
}

std::shared_ptr<ConnectionToClient> ClientNetworkManager::GetNextNewConnection()
{
    if (NewConnections.GetSize() > 0)
    {
        return NewConnections.Pop();
    }
    return nullptr;
}

std::shared_ptr<ConnectionToClient> ClientNetworkManager::GetNextRemovedConnection()
{
    if (RemovedConnections.GetSize() > 0)
    {
        return RemovedConnections.Pop();
    }
    return nullptr;
}
