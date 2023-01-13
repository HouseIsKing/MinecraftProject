#pragma once
#include "Packets/PacketData.h"
#include "Util/ThreadSafeQueue.h"
#include <asio/io_context.hpp>
#include <asio/ip/tcp.hpp>
#include <thread>

class ClientNetworkManager
{
    asio::io_context Context;
    asio::io_context::work IdleWork{Context};
    asio::ip::tcp::socket Socket;
    ThreadSafeQueue<PacketData> Packets;
    std::thread ContextThread = std::thread([&]() { Context.run(); });

public:
    ClientNetworkManager();
    ~ClientNetworkManager();
    ClientNetworkManager(const ClientNetworkManager&) = delete;
    ClientNetworkManager& operator=(const ClientNetworkManager&) = delete;
    ClientNetworkManager(ClientNetworkManager&&) = delete;
    ClientNetworkManager& operator=(ClientNetworkManager&&) = delete;
    void Start(std::string ip, std::string name);
    std::shared_ptr<PacketData> GetNextPacket();
};
