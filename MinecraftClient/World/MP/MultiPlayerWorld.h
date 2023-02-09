#pragma once
/*#include "Network/ClientNetworkManager.h"
#include "Util/States/WorldState.h"
#include "World/Generic/World.h"

class PlayerMp;

class MultiPlayerWorld final : public World
{
    void Init() override;
    ClientNetworkManager NetworkManager;
    uint16_t NextId;
    void HandlePacket(const PacketData* packet);
    void ApplyChangesList(const std::vector<uint8_t>& changes);

public:
    ~MultiPlayerWorld() override;
    MultiPlayerWorld(const MultiPlayerWorld& other) = delete;
    MultiPlayerWorld& operator=(const MultiPlayerWorld& other) = delete;
    MultiPlayerWorld(MultiPlayerWorld&& other) = delete;
    MultiPlayerWorld& operator=(MultiPlayerWorld&& other) = delete;
    std::array<WorldState, EngineDefaults::ROLLBACK_COUNT> WorldStates{};
    MultiPlayerWorld(GLFWwindow* window, const std::string& name, const std::string& ip);
    void Run() override;
    void BuildWorldState();
};*/
