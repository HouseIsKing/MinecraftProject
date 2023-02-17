#pragma once
#include "Network/ClientNetworkManager.h"
#include "World/Generic/ClientWorld.h"

class MultiPlayerWorld final : public ClientWorld
{
    ClientNetworkManager NetworkManager;
    uint64_t LastTickSentData{};

public:
    ~MultiPlayerWorld() override;
    MultiPlayerWorld(const MultiPlayerWorld& other) = delete;
    MultiPlayerWorld& operator=(const MultiPlayerWorld& other) = delete;
    MultiPlayerWorld(MultiPlayerWorld&& other) = delete;
    MultiPlayerWorld& operator=(MultiPlayerWorld&& other) = delete;
    MultiPlayerWorld(GLFWwindow* window, const std::string& name, const std::string& ip);
    void Run() override;
    void NewTick() override;
    void HandlePacket(const PacketData* packet) override;
    void ApplyChangesList(const std::vector<uint8_t>& changes);
};
