//
// Created by amit on 4/21/2022.
//

#include "SinglePlayerWorld.h"
#include "Blocks/GrassBlock.h"
#include <iostream>
#include <cmath>

using std::piecewise_construct;
using std::forward_as_tuple;

SinglePlayerWorld::SinglePlayerWorld(uint16_t width, uint16_t height, uint16_t depth) : levelDepth(depth), levelHeight(height), levelWidth(width),
                                                                                        playerController(0,-5,(float)(height+3),-5) {
    Entity::setWorld(this);
    Chunk::setWorld(this);
    init();
    entities.emplace((uint16_t)0,(Entity*)&playerController);
}

void SinglePlayerWorld::tick() {
    for(auto &entity : entities) {
        entity.second->doTick();
    }
}

void SinglePlayerWorld::init() {
    int amountX = (int)std::ceil((float)levelWidth / (float)Chunk::CHUNK_WIDTH);
    int amountY = (int)std::ceil((float)levelHeight / (float)Chunk::CHUNK_HEIGHT);
    int amountZ = (int)std::ceil((float)levelDepth / (float)Chunk::CHUNK_DEPTH);
    generateChunks(amountX,amountY,amountZ);
    for (int i = 0; i < amountX; i++)
    {
        for (int j = 0; j < amountY; j++)
        {
            for (int k = 0; k < amountZ; k++)
            {
                Chunk* chunk = getChunkAt(i * Chunk::CHUNK_WIDTH, j * Chunk::CHUNK_HEIGHT, k * Chunk::CHUNK_DEPTH);
                for (int x = 0; x < Chunk::CHUNK_WIDTH; x++)
                {
                    for (int y = 0; y < Chunk::CHUNK_HEIGHT; y++)
                    {
                        for (int z = 0; z < Chunk::CHUNK_DEPTH; z++)
                        {
                            chunk->setBlockTypeAt(x, y, z, EBlockType::GRASS);
                        }
                    }
                }
            }
        }
    }
}

void SinglePlayerWorld::generateChunks(int amountX, int amountY, int amountZ) {
    for(int x = 0; x < amountX; x++) {
        for(int y = 0; y < amountY; y++) {
            for(int z = 0; z < amountZ; z++) {
                chunks.emplace(piecewise_construct, forward_as_tuple(x*Chunk::CHUNK_WIDTH,y*Chunk::CHUNK_HEIGHT,z*Chunk::CHUNK_DEPTH), forward_as_tuple(x*Chunk::CHUNK_WIDTH,y*Chunk::CHUNK_HEIGHT,z*Chunk::CHUNK_DEPTH));
            }
        }
    }
}

const Block* SinglePlayerWorld::getBlockAt(int x, int y, int z) {
    return BlockTypeList::getBlockTypeData(getBlockTypeAt(x, y, z));
}

EBlockType SinglePlayerWorld::getBlockTypeAt(int x, int y, int z)
{
    Chunk* chunk = getChunkAt(x, y, z);
    if (chunk == nullptr)
        return EBlockType::AIR;
    return chunk->getBlockTypeAt(x, y, z);
}

ChunkCoords SinglePlayerWorld::getChunkCoords(int x, int y, int z) {
    bool xNeg = x < 0;
    bool yNeg = y < 0;
    bool zNeg = z < 0;
    x/=Chunk::CHUNK_WIDTH;
    y/=Chunk::CHUNK_HEIGHT;
    z/=Chunk::CHUNK_DEPTH;
    if(xNeg) x--;
    if(yNeg) y--;
    if(zNeg) z--;
    x*=Chunk::CHUNK_WIDTH;
    y*=Chunk::CHUNK_HEIGHT;
    z*=Chunk::CHUNK_DEPTH;
    return ChunkCoords{x,y,z};
}

Chunk* SinglePlayerWorld::getChunkAt(int x, int y, int z) {
    ChunkCoords pos = getChunkCoords(x,y,z);
    if(chunks.find(pos) == chunks.end()) {
        return nullptr;
    }
    return &chunks.at(pos);
}

bool SinglePlayerWorld::isBlockExists(int x, int y, int z) {
    return getBlockTypeAt(x, y, z) != EBlockType::AIR;
}

void SinglePlayerWorld::drawWorld() {
    glEnable(GL_DEPTH_TEST);
    for(auto &chunk : chunks) {
        chunk.second.draw();
    }
}

vector<BoundingBox> SinglePlayerWorld::getBlockBoxesInBoundingBox(const BoundingBox& boundingBox) {
    vector<BoundingBox> result{};
    for(int x = (int)boundingBox.getMinX(); (float)x <= boundingBox.getMaxX(); x++) {
        for(int y = (int)boundingBox.getMinY(); (float)y <= boundingBox.getMaxY(); y++) {
            for(int z = (int)boundingBox.getMinZ(); (float)z <= boundingBox.getMaxZ(); z++) {
                BoundingBox helper = getBlockAt(x, y, z)->getBoundingBox();
                helper.move((float)x, (float)y, (float)z);
                result.push_back(helper);
            }
        }
    }
    return result;
}

void SinglePlayerWorld::handleMouseMovementInput(double x, double y) {
    playerController.handleMouseMovementInput((float)x,(float)y);
}

SinglePlayerWorld::~SinglePlayerWorld() = default;
