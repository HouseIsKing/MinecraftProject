#pragma once
#include "World/Generic/ClientWorld.h"
#include <GLFW/glfw3.h>

class SinglePlayerWorld final : public ClientWorld
{
public:
    SinglePlayerWorld(uint16_t width, uint16_t height, uint16_t depth, GLFWwindow* window);
    ~SinglePlayerWorld() override;
    SinglePlayerWorld(const SinglePlayerWorld&) = delete;
    SinglePlayerWorld& operator=(const SinglePlayerWorld&) = delete;
    SinglePlayerWorld(SinglePlayerWorld&&) = delete;
    SinglePlayerWorld& operator=(SinglePlayerWorld&&) = delete;
    void Run() override;
    void NewTick() override;
    void HandlePacket(const PacketData* packet) override;
};
