//
// Created by amit on 4/21/2022.
//
#ifndef MINECRAFTPROJECT_SINGLEPLAYERWORLD_H
#define MINECRAFTPROJECT_SINGLEPLAYERWORLD_H
#include <unordered_map>
#include "ChunkCoords.h"
#include "Chunk.h"
#include "../Entities/Entity.h"
#include "../Entities/Player/PlayerController.h"
#include "Blocks/Block.h"
#include "GLFW/glfw3.h"

using std::unordered_map;
using std::equal_to;

class SinglePlayerWorld {
private:
    unordered_map<ChunkCoords,Chunk,ChunkComparator> chunks;
    unordered_map<uint16_t,Entity*> entities;
    const uint16_t levelWidth;
    const uint16_t levelHeight;
    const uint16_t levelDepth;
    PlayerController playerController;
public:
    SinglePlayerWorld(uint16_t width, uint16_t height, uint16_t depth);
    ~SinglePlayerWorld();
    void tick();
    void handleMouseMovementInput(double x, double y);
    void handleKeyboardPlayerInput(int key, int action);
    void handleWindowResize(int height, int width);
    void init();
    void generateChunks(uint16_t amountX, uint16_t amountY, uint16_t amountZ);
    void generateCaves();
    bool isBlockExists(int x, int y, int z);
    const Block* getBlockAt(int x, int y, int z);
    EBlockType getBlockTypeAt(int x, int y, int z);
    Chunk* getChunkAt(int x, int y, int z);
    vector<BoundingBox> getBlockBoxesInBoundingBox(const BoundingBox& boundingBox);
    void drawWorld();
};
#endif //MINECRAFTPROJECT_SINGLEPLAYERWORLD_H