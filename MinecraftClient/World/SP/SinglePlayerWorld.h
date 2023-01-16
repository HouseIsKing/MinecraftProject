#pragma once
#include "Entities/SP/PlayerController.h"
#include "World/Generic/Chunk.h"
#include "World/Generic/World.h"

class SinglePlayerWorld final : public World<SinglePlayerWorld, PlayerController>
{
    std::stack<uint16_t> EntityAvailableIDs{};
    std::vector<uint16_t> EntitiesToRemove{};
    const uint16_t LevelWidth;
    const uint16_t LevelHeight;
    const uint16_t LevelDepth;
    void SaveWorld();
    void LoadWorld();
    void GenerateChunks(uint16_t amountX, uint16_t amountY, uint16_t amountZ);
    void GenerateLevel();
    void Init() override;
    void RecalculateLightLevels();
    int RecalculateLightLevels(int x, int z);
    void UpdateChunksNear(int x, int y, int z);
    void Tick();

public:
    SinglePlayerWorld(uint16_t width, uint16_t height, uint16_t depth, GLFWwindow* window);
    ~SinglePlayerWorld() override;
    SinglePlayerWorld(const SinglePlayerWorld& other) = delete;
    SinglePlayerWorld& operator=(const SinglePlayerWorld& other) = delete;
    SinglePlayerWorld(SinglePlayerWorld&& other) = delete;
    SinglePlayerWorld& operator=(SinglePlayerWorld&& other) = delete;
    uint16_t RegisterEntity(Entity<SinglePlayerWorld>* entity);
    void RemoveEntity(uint16_t id);
    void PlaceBlockAt(int x, int y, int z, EBlockType blockType);
    void RemoveBlockAt(int x, int y, int z);
    void Run() override;
};
