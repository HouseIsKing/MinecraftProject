//
// Created by amit on 4/21/2022.
//
#pragma once
#include "../Entities/Entity.h"
#include "Chunk.h"
#include "ChunkCoords.h"
#include "GLFW/glfw3.h"
#include <stack>
#include <unordered_map>
#include <unordered_set>

using std::unordered_map;
using std::equal_to;

class MultiPlayerWorld
{
    unordered_map<ChunkCoords, Chunk, ChunkComparator> Chunks;
    unordered_map<uint16_t, unique_ptr<Entity>> Entities;
    std::unordered_set<Chunk*> DirtyChunksDuplicatorCheck{};
    std::vector<Chunk*> DirtyChunks{};
    std::stack<uint16_t> EntityAvailableIDs{};
    std::vector<uint16_t> EntitiesToRemove{};
    long WorldTime; //symbolises world time in ticks
    const uint16_t LevelWidth;
    const uint16_t LevelHeight;
    const uint16_t LevelDepth;
    vector<uint8_t> LightLevels;
    const uint8_t MaxChunkRebuilt = 8;
    float LastTimeFrame;
    float DeltaTime;
    int Frames;
    int Fps;
    void SaveWorld();
    void LoadWorld();
    void GenerateChunks(uint16_t amountX, uint16_t amountY, uint16_t amountZ);
    void GenerateLevel();
    void Init();
    void RecalculateLightLevels();
    int RecalculateLightLevels(int x, int z);
    void UpdateChunksNear(int x, int y, int z);

public:
    MultiPlayerWorld(uint16_t width, uint16_t height, uint16_t depth, GLFWwindow* window);
    ~MultiPlayerWorld();
    MultiPlayerWorld(const SinglePlayerWorld& other) = delete;
    MultiPlayerWorld& operator=(const SinglePlayerWorld& other) = delete;
    MultiPlayerWorld(SinglePlayerWorld&& other) = delete;
    MultiPlayerWorld& operator=(SinglePlayerWorld&& other) = delete;
    uint16_t RegisterEntity(Entity* entity);
    void RemoveEntity(uint16_t id);
    void Tick();
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
};
