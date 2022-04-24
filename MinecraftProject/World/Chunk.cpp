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

Chunk::Chunk(int x, int y, int z) : tessellationHelper(new TessellationHelper(EngineDefaults::getShader())) , chunkPosition(x,y,z), blocks() {
    blocks.reserve(CHUNK_WIDTH*CHUNK_HEIGHT*CHUNK_DEPTH);
    for(int i =0; i < CHUNK_WIDTH; i++) {
        for (int j = 0; j < CHUNK_HEIGHT; j++) {
            for (int k = 0; k < CHUNK_DEPTH; k++) {
                blocks.emplace_back(Block::getAirBlock());
            }
        }
    }
}

Chunk::~Chunk() {
    blocks.clear();
}

void Chunk::drawBlock(Block* block) {
    BoundingBox box = block->getBoundingBox();
    if(!(getWorld()->isBlockExists((int)box.getMinX() + chunkPosition.getX(), (int)box.getMinY() + chunkPosition.getY() + 1, (int)box.getMinZ() + chunkPosition.getZ())))
        block->generateTessellationData(*tessellationHelper,BlockFaces::TOP);
    if(!(getWorld()->isBlockExists((int)box.getMinX() + chunkPosition.getX(), (int)box.getMinY() + chunkPosition.getY() - 1, (int)box.getMinZ() + chunkPosition.getZ())))
        block->generateTessellationData(*tessellationHelper,BlockFaces::BOTTOM);
    if(!(getWorld()->isBlockExists((int)box.getMinX() + chunkPosition.getX(), (int)box.getMinY() + chunkPosition.getY(), (int)box.getMinZ() + chunkPosition.getZ() + 1)))
        block->generateTessellationData(*tessellationHelper,BlockFaces::NORTH);
    if(!(getWorld()->isBlockExists((int)box.getMinX() + chunkPosition.getX(), (int)box.getMinY() + chunkPosition.getY(), (int)box.getMinZ() + chunkPosition.getZ() - 1)))
        block->generateTessellationData(*tessellationHelper,BlockFaces::SOUTH);
    if(!(getWorld()->isBlockExists((int)box.getMinX() + chunkPosition.getX() + 1, (int)box.getMinY() + chunkPosition.getY(), (int)box.getMinZ() + chunkPosition.getZ())))
        block->generateTessellationData(*tessellationHelper,BlockFaces::EAST);
    if(!(getWorld()->isBlockExists((int)box.getMinX() + chunkPosition.getX() - 1, (int)box.getMinY() + chunkPosition.getY(), (int)box.getMinZ() + chunkPosition.getZ())))
        block->generateTessellationData(*tessellationHelper,BlockFaces::WEST);
}

Block* Chunk::getBlockAt(int x, int y, int z) {
    return blocks.at(EngineDefaults::getChunkLocalIndex(x,y,z)).get();
}

void Chunk::resetDraw() {
    tessellationHelper.reset();
}

void Chunk::draw() {
    if(!tessellationHelper->HasInit())
    {
        for(const auto& block : blocks)
        {
            if(!block->isAirBlock())
                drawBlock(block.get());
        }
    }
    tessellationHelper->draw();
}

void Chunk::setBlockAt(int x, int y, int z, Block* block) {
    blocks.at(EngineDefaults::getChunkLocalIndex(x,y,z)).reset(block);
}
