#pragma once
#include "Entities/MP/PlayerMp.h"
#include "Network/ClientNetworkManager.h"
#include "World/Generic/World.h"

class MultiPlayerWorld final : public World<MultiPlayerWorld, PlayerMp>
{
    void Init() override;
    void UpdateChunksNear(int x, int y, int z);
    ClientNetworkManager NetworkManager;
    uint16_t NextId;
    void HandlePacket(const PacketData* packet);

public:
    ~MultiPlayerWorld() override;
    MultiPlayerWorld(const MultiPlayerWorld& other) = delete;
    MultiPlayerWorld& operator=(const MultiPlayerWorld& other) = delete;
    MultiPlayerWorld(MultiPlayerWorld&& other) = delete;
    MultiPlayerWorld& operator=(MultiPlayerWorld&& other) = delete;
    MultiPlayerWorld(GLFWwindow* window, const std::string& name, const std::string& ip);
    uint16_t RegisterEntity(Entity<MultiPlayerWorld>* entity);
    void Run() override;
};
