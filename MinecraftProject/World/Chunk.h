//
// Created by amit on 4/21/2022.
//

#ifndef MINECRAFTPROJECT_CHUNK_H
#define MINECRAFTPROJECT_CHUNK_H
#include "ChunkCoords.h"
#include "../Util/TessellationHelper.h"
#include <array>
#include "Blocks/Block.h"

class SinglePlayerWorld;

using std::array;
using std::unique_ptr;

class Chunk {
public:
    static const char CHUNK_WIDTH = 16;
    static const char CHUNK_HEIGHT = 16;
    static const char CHUNK_DEPTH = 16;
    static void setWorld(SinglePlayerWorld* newWorld);
    static SinglePlayerWorld* getWorld();
    Chunk(int x, int y, int z);
    Block* getBlockAt(int x, int y, int z);
    void resetDraw();
    void draw();
    void setBlockAt(int x, int y, int z, Block* block);
    ~Chunk();
private:
    ChunkCoords chunkPosition;
    unique_ptr<TessellationHelper> tessellationHelper;
    static SinglePlayerWorld* world;
    vector<unique_ptr<Block>> blocks;
    void drawBlock(Block* block);

};
#endif //MINECRAFTPROJECT_CHUNK_H
