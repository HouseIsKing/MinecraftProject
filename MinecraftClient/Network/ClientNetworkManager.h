#pragma once
#include "Packets/PacketData.h"
#include "Util/ThreadSafeQueue.h"
#include <asio/io_context.hpp>
#include <asio/ip/tcp.hpp>
#include <thread>

class ServerNetworkManager
{
    asio::io_context Context;
    asio::io_context::work IdleWork{Context};
    asio::ip::tcp::socket Socket;
    ThreadSafeQueue<PacketData> Packets;
    std::thread ContextThread = std::thread([&]() { Context.run(); });

public:
    ServerNetworkManager();
    ~ServerNetworkManager();
    ServerNetworkManager(const ServerNetworkManager&) = delete;
    ServerNetworkManager& operator=(const ServerNetworkManager&) = delete;
    ServerNetworkManager(ServerNetworkManager&&) = delete;
    ServerNetworkManager& operator=(ServerNetworkManager&&) = delete;
    void Start(std::string ip, std::string name);
    std::shared_ptr<PacketData> GetNextPacket();
};
