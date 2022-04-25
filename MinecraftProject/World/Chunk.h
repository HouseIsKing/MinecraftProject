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
    const Block* getBlockAt(int x, int y, int z);
    EBlockType getBlockTypeAt(int x, int y, int z);
    bool isCoordsInsideChunk(int x, int y, int z);
    void resetDraw();
    void draw();
    void setBlockTypeAt(int x, int y, int z, EBlockType block);
private:
    ChunkCoords chunkPosition;
    unique_ptr<TessellationHelper> tessellationHelper;
    static SinglePlayerWorld* world;
    EBlockType blocks[CHUNK_HEIGHT*CHUNK_DEPTH*CHUNK_WIDTH]{EBlockType::AIR};
    void drawBlock(EBlockType blockType, int x, int y, int z);

};
#endif //MINECRAFTPROJECT_CHUNK_H
