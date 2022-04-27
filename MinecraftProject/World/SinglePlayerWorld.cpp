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
                                                                                        playerController(0,5,(float)(height+3),5) {
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

void SinglePlayerWorld::handleWindowResize(int height, int width)
{
    glViewport(0, 0, width, height);
    CameraController::onResizeWindow(width, height);
}

void SinglePlayerWorld::init() {
    uint16_t amountX = (uint16_t)std::ceil((float)levelWidth / (float)Chunk::CHUNK_WIDTH);
    uint16_t amountY = (uint16_t)std::ceil((float)levelHeight / (float)Chunk::CHUNK_HEIGHT);
    uint16_t amountZ = (uint16_t)std::ceil((float)levelDepth / (float)Chunk::CHUNK_DEPTH);
    generateChunks(amountX,amountY,amountZ);
    generateCaves();
}

void SinglePlayerWorld::generateChunks(uint16_t amountX, uint16_t amountY, uint16_t amountZ) {
    chunks.reserve((uint64_t)amountX * (uint64_t)amountY * (uint64_t)amountZ);
    for(int x = 0; x < amountX; x++) {
        for(int y = 0; y < amountY; y++) {
            for(int z = 0; z < amountZ; z++) {
                chunks.emplace(piecewise_construct, forward_as_tuple(x*Chunk::CHUNK_WIDTH,y*Chunk::CHUNK_HEIGHT,z*Chunk::CHUNK_DEPTH), forward_as_tuple(x*Chunk::CHUNK_WIDTH,y*Chunk::CHUNK_HEIGHT,z*Chunk::CHUNK_DEPTH));
            }
        }
    }
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

void SinglePlayerWorld::generateCaves()
{
    for (int i = 0; i < 10000; i++)
    {
        uint_fast8_t caveSize = EngineDefaults::getNext<uint_fast8_t>(1,8);
        uint_fast16_t caveX = EngineDefaults::getNext<uint16_t>(levelWidth);
        uint_fast16_t caveY = EngineDefaults::getNext<uint16_t>(levelHeight);
        uint_fast16_t caveZ = EngineDefaults::getNext<uint16_t>(levelDepth);
        for (uint_fast32_t radius = 1; radius < caveSize; radius++)
        {
            for (uint_fast16_t sphere = 0; sphere < 1000; sphere++)
            {
                int offsetX = EngineDefaults::getNext<uint_fast16_t>(radius*2)-radius;
                int offsetY = EngineDefaults::getNext<uint_fast16_t>(radius*2)-radius;
                int offsetZ = EngineDefaults::getNext<uint_fast16_t>(radius*2)-radius;
                uint_fast32_t distance = offsetX*offsetX + offsetY*offsetY + offsetZ*offsetZ;
                if (distance <= radius * radius)
                {
                    int tileX = caveX + offsetX;
                    int tileY = caveY + offsetY;
                    int tileZ = caveZ + offsetZ;
                    if (tileX >= 0 && tileX < levelWidth && tileY >= 0 && tileY < levelHeight && tileZ >= 0 && tileZ < levelDepth)
                    {
                        getChunkAt(tileX, tileY, tileZ)->setBlockTypeAt(tileX, tileY, tileZ, EBlockType::AIR);
                    }
                }
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

Chunk* SinglePlayerWorld::getChunkAt(int x, int y, int z) {
    ChunkCoords pos = ChunkCoords(x,y,z);
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
    glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
    for(auto &chunk : chunks) {
        chunk.second.draw();
    }
	glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);
}

vector<BoundingBox> SinglePlayerWorld::getBlockBoxesInBoundingBox(const BoundingBox& boundingBox) {
    vector<BoundingBox> result{};
    for(int x = (int)boundingBox.getMinX(); (float)x <= boundingBox.getMaxX(); x++) {
        for(int y = (int)boundingBox.getMinY(); (float)y <= boundingBox.getMaxY(); y++) {
            for(int z = (int)boundingBox.getMinZ(); (float)z <= boundingBox.getMaxZ(); z++) {
                EBlockType block = getBlockTypeAt(x, y, z);
                if (block == EBlockType::AIR)
                {
                    continue;
                }
                BoundingBox helper = BlockTypeList::getBlockTypeData(block)->getBoundingBox();
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

void SinglePlayerWorld::handleKeyboardPlayerInput(int key, int action)
{
    playerController.handleKeyboardMovementInput(key, action);
}

SinglePlayerWorld::~SinglePlayerWorld() = default;
