#pragma once
#include "Network/ConnectionToClientInterface.h"
#include "Network/ServerNetworkManager.h"
#include "World/World.h"

class ServerManager final : World
{
    ServerNetworkManager NetworkManager;
    std::unordered_map<std::shared_ptr<ConnectionToClientInterface>, Player*, ConnectionHasher, ConnectionEqual> Connections{};
    std::vector<uint16_t> DisconnectedClients{};

public:
    ~ServerManager() override;
    ServerManager(const ServerManager&) = delete;
    ServerManager(ServerManager&&) = delete;
    ServerManager& operator=(const ServerManager&) = delete;
    ServerManager& operator=(ServerManager&&) = delete;
    ServerManager(uint16_t width, uint16_t height, uint16_t depth);
    void Run() override;
    void NewTick() override;
    void HandlePacket(const PacketData* packet) override;
    void PreTick() override;
};
