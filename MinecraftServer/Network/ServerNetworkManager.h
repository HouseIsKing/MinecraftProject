#pragma once
#include "ConnectionToClient.h"
#include <asio/io_context.hpp>
#include <queue>
#include <unordered_map>

#include "Entities/Player.h"

class ServerNetworkManager
{
    asio::io_context Context;
    std::unordered_map<Player, ConnectionToClient, PlayerHasher> Connections;
    asio::ip::tcp::acceptor Acceptor;
    std::queue<ConnectionToClient> NewConnections;
    std::queue<ConnectionToClient> DisconnectedConnections;

    void RunAccepptor();
    void RunReadMessages();

public:
    ServerNetworkManager();
    ~ServerNetworkManager();
    ServerNetworkManager(const ServerNetworkManager&) = delete;
    ServerNetworkManager& operator=(const ServerNetworkManager&) = delete;
    ServerNetworkManager(ServerNetworkManager&&) = delete;
    ServerNetworkManager& operator=(ServerNetworkManager&&) = delete;
    void Start();
};
