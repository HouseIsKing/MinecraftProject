//
// Created by amit on 4/21/2022.
//

#include "Chunk.h"
#include "../Util/EngineDefaults.h"
#include "SinglePlayerWorld.h"

SinglePlayerWorld* Chunk::world = nullptr;

void Chunk::setWorld(SinglePlayerWorld *newWorld) {
    world = newWorld;
}

SinglePlayerWorld *Chunk::getWorld() {
    return world;
}

Chunk::Chunk(int x, int y, int z) : tessellationHelper(new TessellationHelper(EngineDefaults::getShader())) , chunkPosition(x,y,z) {
    for (int i = 0; i < CHUNK_WIDTH*CHUNK_DEPTH*CHUNK_HEIGHT; i++)
    {
        blocks[i] = EBlockType::AIR;
    }
}

void Chunk::drawBlock(EBlockType blockType, int x, int y, int z) {
    const Block* myBlock = BlockTypeList::getBlockTypeData(blockType);
    int finalX = x + chunkPosition.getX();
    int finalY = y + chunkPosition.getY();
    int finalZ = z + chunkPosition.getZ();
    if (isCoordsInsideChunk(finalX, finalY + 1, finalZ))
    {
        if (blocks[EngineDefaults::getChunkLocalIndex(finalX, finalY + 1, finalZ)] == EBlockType::AIR)
        {
            myBlock->generateTessellationData(*tessellationHelper, BlockFaces::TOP, (float)finalX, (float)finalY, (float)finalZ);
        }
    }
    else if (!(getWorld()->isBlockExists(finalX, finalY + 1, finalZ)))
    {
        myBlock->generateTessellationData(*tessellationHelper, BlockFaces::TOP, (float)finalX, (float)finalY, (float)finalZ);
    }
    if (isCoordsInsideChunk(finalX, finalY - 1, finalZ))
    {
        if (blocks[EngineDefaults::getChunkLocalIndex(finalX, finalY - 1, finalZ)] == EBlockType::AIR)
        {
            myBlock->generateTessellationData(*tessellationHelper, BlockFaces::BOTTOM, (float)finalX, (float)finalY, (float)finalZ);
        }
    }
    else if (!(getWorld()->isBlockExists(finalX, finalY - 1, finalZ)))
    {
        myBlock->generateTessellationData(*tessellationHelper, BlockFaces::BOTTOM, (float)finalX, (float)finalY, (float)finalZ);
    }
    if (isCoordsInsideChunk(finalX, finalY, finalZ + 1))
    {
        if (blocks[EngineDefaults::getChunkLocalIndex(finalX, finalY, finalZ + 1)] == EBlockType::AIR)
        {
            myBlock->generateTessellationData(*tessellationHelper, BlockFaces::NORTH, (float)finalX, (float)finalY, (float)finalZ);
        }
    }
    else if (!(getWorld()->isBlockExists(finalX, finalY, finalZ + 1)))
    {
        myBlock->generateTessellationData(*tessellationHelper, BlockFaces::NORTH, (float)finalX, (float)finalY, (float)finalZ);
    }
    if (isCoordsInsideChunk(finalX, finalY, finalZ - 1))
    {
        if (blocks[EngineDefaults::getChunkLocalIndex(finalX, finalY, finalZ - 1)] == EBlockType::AIR)
        {
            myBlock->generateTessellationData(*tessellationHelper, BlockFaces::SOUTH, (float)finalX, (float)finalY, (float)finalZ);
        }
    }
    else if (!(getWorld()->isBlockExists(finalX, finalY, finalZ - 1)))
    {
        myBlock->generateTessellationData(*tessellationHelper, BlockFaces::SOUTH, (float)finalX, (float)finalY, (float)finalZ);
    }
    if (isCoordsInsideChunk(finalX + 1, finalY, finalZ))
    {
        if (blocks[EngineDefaults::getChunkLocalIndex(finalX + 1, finalY, finalZ)] == EBlockType::AIR)
        {
            myBlock->generateTessellationData(*tessellationHelper, BlockFaces::EAST, (float)finalX, (float)finalY, (float)finalZ);
        }
    }
    else if (!(getWorld()->isBlockExists(finalX + 1, finalY, finalZ)))
    {
        myBlock->generateTessellationData(*tessellationHelper, BlockFaces::EAST, (float)finalX, (float)finalY, (float)finalZ);
    }
    if (isCoordsInsideChunk(finalX - 1, finalY, finalZ))
    {
        if (blocks[EngineDefaults::getChunkLocalIndex(finalX - 1, finalY, finalZ)] == EBlockType::AIR)
        {
            myBlock->generateTessellationData(*tessellationHelper, BlockFaces::WEST, (float)finalX, (float)finalY, (float)finalZ);
        }
    }
    else if (!(getWorld()->isBlockExists(finalX - 1, finalY, finalZ)))
    {
        myBlock->generateTessellationData(*tessellationHelper, BlockFaces::WEST, (float)finalX, (float)finalY, (float)finalZ);
    }
}

const Block* Chunk::getBlockAt(int x, int y, int z) {
    return BlockTypeList::getBlockTypeData(getBlockTypeAt(x,y,z));
}

EBlockType Chunk::getBlockTypeAt(int x, int y, int z)
{
    return blocks[EngineDefaults::getChunkLocalIndex(x, y, z)];
}

bool Chunk::isCoordsInsideChunk(int x, int y, int z)
{
    return x >= chunkPosition.getX() && y >= chunkPosition.getY() && z >= chunkPosition.getZ() && x < chunkPosition.getX() + CHUNK_WIDTH && y < chunkPosition.getY() + CHUNK_HEIGHT && z < chunkPosition.getZ() + CHUNK_DEPTH;
}

void Chunk::resetDraw() {
    tessellationHelper.reset();
}

void Chunk::draw() {
    if(!tessellationHelper->HasInit())
    {
        for (int x = 0; x < CHUNK_WIDTH; x++)
        {
            for (int y = 0; y < CHUNK_HEIGHT; y++)
            {
                for (int z = 0; z < CHUNK_DEPTH; z++)
                {
                    EBlockType type = blocks[EngineDefaults::getChunkLocalIndex(x, y, z)];
                    if (type != EBlockType::AIR)
                        drawBlock(type, x, y, z);
                }
            }
        }
    }
    tessellationHelper->draw();
}

void Chunk::setBlockTypeAt(int x, int y, int z, EBlockType block) {
    blocks[EngineDefaults::getChunkLocalIndex(x, y, z)] = block;
    if (tessellationHelper->HasInit())
        tessellationHelper->reset();
}
