#pragma once
#include "Packets/PacketData.h"
#include "Util/ThreadSafeQueue.h"
#include "Util/ThreadSafeSet.h"
#include <asio/io_context.hpp>
#include <asio/ip/tcp.hpp>
#include <thread>

class ServerNetworkManager
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
    ServerNetworkManager();
    ~ServerNetworkManager();
    ServerNetworkManager(const ServerNetworkManager&) = delete;
    ServerNetworkManager& operator=(const ServerNetworkManager&) = delete;
    ServerNetworkManager(ServerNetworkManager&&) = delete;
    ServerNetworkManager& operator=(ServerNetworkManager&&) = delete;
    void AddPacket(const std::shared_ptr<PacketData>& packet);
    void AddRemovedConnection(const std::shared_ptr<ConnectionToClient>& connection);
    void Start();
    std::shared_ptr<PacketData> GetNextPacket();
    std::shared_ptr<ConnectionToClient> GetNextNewConnection();
    std::shared_ptr<ConnectionToClient> GetNextRemovedConnection();
};
