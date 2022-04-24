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
    double timeStart = glfwGetTime();
    generateChunks(amountX,amountY,amountZ);
    double timeEnd = glfwGetTime();
    std::cout << "Generated chunks in " << timeEnd - timeStart << " seconds" << std::endl;
    for(int i =0;i<levelWidth;i++)
    {
        for(int j =0;j<levelHeight;j++)
        {
            for(int k =0;k<levelDepth;k++)
            {
                getChunkAt(i,j,k)->setBlockAt(i,j,k, new GrassBlock((float)i,(float)j,(float)k));
            }
        }
    }
}

void SinglePlayerWorld::generateChunks(int amountX, int amountY, int amountZ) {
    for(int x = 0; x < amountX; x++) {
        for(int y = 0; y < amountY; y++) {
            for(int z = 0; z < amountZ; z++) {
                double timeStart = glfwGetTime();
                chunks.emplace(piecewise_construct, forward_as_tuple(x*Chunk::CHUNK_WIDTH,y*Chunk::CHUNK_HEIGHT,z*Chunk::CHUNK_DEPTH), forward_as_tuple(x*Chunk::CHUNK_WIDTH,y*Chunk::CHUNK_HEIGHT,z*Chunk::CHUNK_DEPTH));
                double timeEnd = glfwGetTime();
                std::cout << "Generated chunk in " << timeEnd - timeStart << " seconds" << std::endl;
            }
        }
    }
}

Block* SinglePlayerWorld::getBlockAt(int x, int y, int z) {
    Chunk* chunk = getChunkAt(x,y,z);
    if(chunk == nullptr) {
        return Block::getAirBlock();
    }
    return chunk->getBlockAt(x,y,z);
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
    return !getBlockAt(x,y,z)->isAirBlock();
}

void SinglePlayerWorld::drawWorld() {
    glEnable(GL_DEPTH_TEST);
    for(auto &chunk : chunks) {
        chunk.second.draw();
    }
}

vector<Block*> SinglePlayerWorld::getBlocksInBoundingBox(const BoundingBox& boundingBox) {
    vector<Block*> result{};
    for(int x = (int)boundingBox.getMinX(); (float)x <= boundingBox.getMaxX(); x++) {
        for(int y = (int)boundingBox.getMinY(); (float)y <= boundingBox.getMaxY(); y++) {
            for(int z = (int)boundingBox.getMinZ(); (float)z <= boundingBox.getMaxZ(); z++) {
                if(isBlockExists(x,y,z)) {
                    result.push_back(getBlockAt(x,y,z));
                }
            }
        }
    }
    return result;
}

void SinglePlayerWorld::handleMouseMovementInput(double x, double y) {
    playerController.handleMouseMovementInput((float)x,(float)y);
}

SinglePlayerWorld::~SinglePlayerWorld() = default;
