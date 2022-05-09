//
// Created by amit on 4/21/2022.
//
#pragma once
#include "../Entities/Entity.h"
#include "../Entities/Player/PlayerController.h"
#include "Blocks/Block.h"
#include "Chunk.h"
#include "ChunkCoords.h"
#include "GLFW/glfw3.h"
#include <unordered_map>

using std::unordered_map;
using std::equal_to;

class SinglePlayerWorld
{
    unordered_map<ChunkCoords, Chunk, ChunkComparator> Chunks;
    unordered_map<uint16_t, Entity*> Entities;
    const uint16_t LevelWidth;
    const uint16_t LevelHeight;
    const uint16_t LevelDepth;
    PlayerController PlayerController;
public:
    SinglePlayerWorld(uint16_t width, uint16_t height, uint16_t depth);
    ~SinglePlayerWorld();
    SinglePlayerWorld(const SinglePlayerWorld& other) = delete;
    SinglePlayerWorld& operator=(const SinglePlayerWorld& other) = delete;
    SinglePlayerWorld(SinglePlayerWorld&& other) = delete;
    SinglePlayerWorld& operator=(SinglePlayerWorld&& other) = delete;
    void Tick();
    void HandleMouseMovementInput(double x, double y);
    void HandleKeyboardPlayerInput(int key, int action);
    static void HandleWindowResize(int height, int width);
    void Init();
    void GenerateChunks(uint16_t amountX, uint16_t amountY, uint16_t amountZ);
    void GenerateCaves();
    bool IsBlockExists(int x, int y, int z);
    const Block* GetBlockAt(int x, int y, int z);
    EBlockType GetBlockTypeAt(int x, int y, int z);
    Chunk* GetChunkAt(int x, int y, int z);
    vector<BoundingBox> GetBlockBoxesInBoundingBox(const BoundingBox& boundingBox);
    void DrawWorld();
};