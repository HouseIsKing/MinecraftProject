#pragma once
#include "Entities/MP/PlayerMp.h"
#include "World/Generic/World.h"

class MultiPlayerWorld final : public World<MultiPlayerWorld, PlayerMp>
{
    void Init() override;
    void UpdateChunksNear(int x, int y, int z);

public:
    explicit MultiPlayerWorld(GLFWwindow* window);
    ~MultiPlayerWorld() override;
    MultiPlayerWorld(const MultiPlayerWorld& other) = delete;
    MultiPlayerWorld& operator=(const MultiPlayerWorld& other) = delete;
    MultiPlayerWorld(MultiPlayerWorld&& other) = delete;
    MultiPlayerWorld& operator=(MultiPlayerWorld&& other) = delete;
    uint16_t RegisterEntity(Entity<MultiPlayerWorld>* entity);
    void Run() override;
};
