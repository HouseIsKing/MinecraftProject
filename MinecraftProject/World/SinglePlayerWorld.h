//
// Created by amit on 4/21/2022.
//
#pragma once
#include "../Entities/Entity.h"
#include "../Entities/Player/PlayerController.h"
#include "Chunk.h"
#include "ChunkCoords.h"
#include "GLFW/glfw3.h"
#include <stack>
#include <unordered_map>
#include <unordered_set>

#include "GUI/Gui.h"

using std::unordered_map;
using std::equal_to;

class SinglePlayerWorld
{
    unordered_map<ChunkCoords, Chunk, ChunkComparator> Chunks;
    unordered_map<uint16_t, unique_ptr<Entity>> Entities;
    std::unordered_set<Chunk*> DirtyChunksDuplicatorCheck{};
    std::vector<Chunk*> DirtyChunks{};
    std::stack<uint16_t> EntityAvailableIDs{};
    std::vector<uint16_t> EntitiesToRemove{};
    vector<unique_ptr<Gui>> Guis;
    PlayerController* Player;
    long WorldTime; //symbolises world time in ticks
    const uint16_t LevelWidth;
    const uint16_t LevelHeight;
    const uint16_t LevelDepth;
    GLuint FogsBuffer;
    GLFWwindow* TheAppWindow;
    vector<uint8_t> LightLevels;
    const uint8_t MaxChunkRebuilt = 8;
    void SaveWorld();
    void LoadWorld();
    void GenerateChunks(uint16_t amountX, uint16_t amountY, uint16_t amountZ);
    void GenerateLevel();
    void Init();
    void InitFog();
    void RecalculateLightLevels();
    int RecalculateLightLevels(int x, int z);
    void UpdateChunksNear(int x, int y, int z);
    void DrawGui() const;
    void RebuildGui() const;

public:
    SinglePlayerWorld(uint16_t width, uint16_t height, uint16_t depth, GLFWwindow* window);
    ~SinglePlayerWorld();
    SinglePlayerWorld(const SinglePlayerWorld& other) = delete;
    SinglePlayerWorld& operator=(const SinglePlayerWorld& other) = delete;
    SinglePlayerWorld(SinglePlayerWorld&& other) = delete;
    SinglePlayerWorld& operator=(SinglePlayerWorld&& other) = delete;
    uint16_t RegisterEntity(Entity* entity);
    void RemoveEntity(uint16_t id);
    void Tick();
    void HandleWindowResize(int height, int width) const;
    bool IsBlockExists(int x, int y, int z);
    void PlaceBlockAt(int x, int y, int z, EBlockType blockType);
    void RemoveBlockAt(int x, int y, int z);
    void AddChunkAsDirty(Chunk* chunk);
    [[nodiscard]] Entity* GetEntity(uint16_t id) const;
    [[nodiscard]] PlayerController* GetPlayer() const;
    const Block* GetBlockAt(int x, int y, int z);
    EBlockType GetBlockTypeAt(int x, int y, int z);
    Chunk* GetChunkAt(int x, int y, int z);
    [[nodiscard]] int GetBrightnessAt(int x, int y, int z) const;
    bool IsBlockSolid(int x, int y, int z);
    [[nodiscard]] int GetBrightnessAt(vec3 pos) const;
    vector<BoundingBox> GetBlockBoxesInBoundingBox(const BoundingBox& boundingBox);
    void DrawWorld(float partialTick);
    [[nodiscard]] GLFWwindow* GetWindow() const;
    template <typename T>
    [[nodiscard]] Gui* GetGuiOfType() const;
};

template <typename T>
Gui* SinglePlayerWorld::GetGuiOfType() const
{
    for (auto& gui : Guis)
    {
        if (dynamic_cast<T*>(gui.get()))
        {
            return gui.get();
        }
    }
    return nullptr;
}
