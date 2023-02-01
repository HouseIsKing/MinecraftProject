#pragma once
#include "Entities/SP/PlayerController.h"
#include "World/Generic/Chunk.h"
#include "World/Generic/World.h"

class SinglePlayerWorld final : public World
{
    void SaveWorld();
    void LoadWorld();
    void GenerateChunks(uint16_t amountX, uint16_t amountY, uint16_t amountZ);
    void GenerateLevel();
    void Init() override;
    void RecalculateLightLevels();

public:
    SinglePlayerWorld(uint16_t width, uint16_t height, uint16_t depth, GLFWwindow* window);
    ~SinglePlayerWorld() override;
    SinglePlayerWorld(const SinglePlayerWorld& other) = delete;
    SinglePlayerWorld& operator=(const SinglePlayerWorld& other) = delete;
    SinglePlayerWorld(SinglePlayerWorld&& other) = delete;
    SinglePlayerWorld& operator=(SinglePlayerWorld&& other) = delete;
    void Run() override;
};
