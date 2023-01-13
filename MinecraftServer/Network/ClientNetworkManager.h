#pragma once
#include "Packets/PacketData.h"
#include "Util/ThreadSafeQueue.h"
#include "Util/ThreadSafeSet.h"
#include <asio/io_context.hpp>
#include <asio/ip/tcp.hpp>
#include <thread>

class ClientNetworkManager
{
    asio::io_context Context;
    asio::io_context::work IdleWork{Context};
    asio::ip::tcp::acceptor Acceptor;
    ThreadSafeQueue<PacketData> Packets;
    std::shared_ptr<ConnectionToClient> NewConnection;
    ThreadSafeQueue<ConnectionToClient> NewConnections;
    ThreadSafeQueue<ConnectionToClient> RemovedConnections;
    std::thread ContextThread = std::thread([&]() { Context.run(); });

    void RunAcceptor();

public:
    ClientNetworkManager();
    ~ClientNetworkManager();
    ClientNetworkManager(const ClientNetworkManager&) = delete;
    ClientNetworkManager& operator=(const ClientNetworkManager&) = delete;
    ClientNetworkManager(ClientNetworkManager&&) = delete;
    ClientNetworkManager& operator=(ClientNetworkManager&&) = delete;
    void AddPacket(const std::shared_ptr<PacketData>& packet);
    void Start();
    std::shared_ptr<PacketData> GetNextPacket();
    std::shared_ptr<ConnectionToClient> GetNextNewConnection();
    std::shared_ptr<ConnectionToClient> GetNextRemovedConnection();
};
