//
// Created by amit on 4/21/2022.
//

#include "SinglePlayerWorld.h"
#include "../Util/CustomFileManager.h"
#include "../Util/EngineDefaults.h"
#include "Blocks/GrassBlock.h"
#include <cmath>
#include <filesystem>
#include <ranges>

using std::piecewise_construct;
using std::forward_as_tuple;


void SinglePlayerWorld::SaveWorld()
{
    CustomFileManager fileManager{"level.dat", "w"};
    fileManager << Chunks.size();
    for (auto& val : Chunks | std::views::values)
    {
        fileManager << val;
    }
}

void SinglePlayerWorld::LoadWorld()
{
    CustomFileManager fileManager{"level.dat", "r"};
    size_t size;
    fileManager >> size;
    for (size_t i = 0; i < size; i++)
    {
        ChunkCoords coords;
        fileManager >> coords;
        int x = coords.GetX() * Chunk::CHUNK_WIDTH;
        int y = coords.GetY() * Chunk::CHUNK_HEIGHT;
        int z = coords.GetZ() * Chunk::CHUNK_DEPTH;
        fileManager >> Chunks.emplace(piecewise_construct, forward_as_tuple(x, y, z), forward_as_tuple(x, y, z)).first->second;
    }
}

SinglePlayerWorld::SinglePlayerWorld(const uint16_t width, const uint16_t height, const uint16_t depth) : LevelWidth(width), LevelHeight(height), LevelDepth(depth), PlayerController(0, 5, static_cast<float>(height + 3), 5)
{
    Entity::SetWorld(this);
    Chunk::SetWorld(this);
    Init();
    Entities.emplace(static_cast<uint16_t>(0), reinterpret_cast<Entity*>(&PlayerController));
}

void SinglePlayerWorld::Tick()
{
    for (const auto& val : Entities | std::views::values)
    {
        val->DoTick();
    }
}

void SinglePlayerWorld::HandleWindowResize(const int height, const int width)
{
    glViewport(0, 0, width, height);
    CameraController::OnResizeWindow(width, height);
}

void SinglePlayerWorld::Init()
{
    if (std::filesystem::exists("level.dat"))
    {
        LoadWorld();
    }
    else
    {
        const auto amountX = static_cast<uint16_t>(std::ceil(static_cast<float>(LevelWidth) / static_cast<float>(Chunk::CHUNK_WIDTH)));
        const auto amountY = static_cast<uint16_t>(std::ceil(static_cast<float>(LevelHeight) / static_cast<float>(Chunk::CHUNK_HEIGHT)));
        const auto amountZ = static_cast<uint16_t>(std::ceil(static_cast<float>(LevelDepth) / static_cast<float>(Chunk::CHUNK_DEPTH)));
        GenerateChunks(amountX, amountY, amountZ);
        GenerateCaves();
    }
}

void SinglePlayerWorld::GenerateChunks(const uint16_t amountX, const uint16_t amountY, const uint16_t amountZ)
{
    Chunks.reserve(static_cast<uint64_t>(amountX) * static_cast<uint64_t>(amountY) * static_cast<uint64_t>(amountZ));
    for (int x = 0; x < amountX; x++)
    {
        for (int y = 0; y < amountY; y++)
        {
            for (int z = 0; z < amountZ; z++)
            {
                Chunks.emplace(piecewise_construct, forward_as_tuple(x * Chunk::CHUNK_WIDTH, y * Chunk::CHUNK_HEIGHT, z * Chunk::CHUNK_DEPTH), forward_as_tuple(x * Chunk::CHUNK_WIDTH, y * Chunk::CHUNK_HEIGHT, z * Chunk::CHUNK_DEPTH));
            }
        }
    }
    for (int i = 0; i < amountX; i++)
    {
        for (int j = 0; j < amountY; j++)
        {
            for (int k = 0; k < amountZ; k++)
            {
                Chunk* chunk = GetChunkAt(i * Chunk::CHUNK_WIDTH, j * Chunk::CHUNK_HEIGHT, k * Chunk::CHUNK_DEPTH);
                for (int x = 0; x < Chunk::CHUNK_WIDTH; x++)
                {
                    for (int y = 0; y < Chunk::CHUNK_HEIGHT; y++)
                    {
                        for (int z = 0; z < Chunk::CHUNK_DEPTH; z++)
                        {
                            chunk->SetBlockTypeAt(x, y, z, EBlockType::Grass);
                        }
                    }
                }
            }
        }
    }
}

void SinglePlayerWorld::GenerateCaves()
{
    for (int i = 0; i < 10000; i++)
    {
        const auto caveSize = EngineDefaults::GetNext<uint_fast8_t>(1, 8);
        const uint_fast16_t caveX = EngineDefaults::GetNext<uint16_t>(LevelWidth);
        const uint_fast16_t caveY = EngineDefaults::GetNext<uint16_t>(LevelHeight);
        const uint_fast16_t caveZ = EngineDefaults::GetNext<uint16_t>(LevelDepth);
        for (uint_fast32_t radius = 1; radius < caveSize; radius++)
        {
            for (uint_fast16_t sphere = 0; sphere < 1000; sphere++)
            {
                const int offsetX = static_cast<int>(EngineDefaults::GetNext<uint_fast16_t>(radius * 2) - radius);
                const int offsetY = static_cast<int>(EngineDefaults::GetNext<uint_fast16_t>(radius * 2) - radius);
                const int offsetZ = static_cast<int>(EngineDefaults::GetNext<uint_fast16_t>(radius * 2) - radius);
                if (const auto distance = static_cast<uint_fast32_t>(offsetX * offsetX + offsetY * offsetY + offsetZ * offsetZ); distance <= radius * radius)
                {
                    const int tileX = static_cast<int>(caveX) + offsetX;
                    const int tileY = static_cast<int>(caveY) + offsetY;
                    if (const int tileZ = static_cast<int>(caveZ) + offsetZ; tileX >= 0 && tileX < LevelWidth && tileY >= 0 && tileY < LevelHeight && tileZ >= 0 && tileZ < LevelDepth)
                    {
                        GetChunkAt(tileX, tileY, tileZ)->SetBlockTypeAt(tileX, tileY, tileZ, EBlockType::Air);
                    }
                }
            }
        }
    }
}

const Block* SinglePlayerWorld::GetBlockAt(const int x, const int y, const int z)
{
    return BlockTypeList::GetBlockTypeData(GetBlockTypeAt(x, y, z));
}

EBlockType SinglePlayerWorld::GetBlockTypeAt(const int x, const int y, const int z)
{
    const Chunk* chunk = GetChunkAt(x, y, z);
    if (chunk == nullptr)
    {
        return EBlockType::Air;
    }
    return chunk->GetBlockTypeAt(x, y, z);
}

Chunk* SinglePlayerWorld::GetChunkAt(const int x, const int y, const int z)
{
    const auto pos = ChunkCoords(x, y, z);
    if (!Chunks.contains(pos))
    {
        return nullptr;
    }
    return &Chunks.at(pos);
}

bool SinglePlayerWorld::IsBlockExists(const int x, const int y, const int z)
{
    return GetBlockTypeAt(x, y, z) != EBlockType::Air;
}

void SinglePlayerWorld::DrawWorld()
{
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    for (auto& val : Chunks | std::views::values)
    {
        val.Draw();
    }
	glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);
}

vector<BoundingBox> SinglePlayerWorld::GetBlockBoxesInBoundingBox(const BoundingBox& boundingBox)
{
    vector<BoundingBox> result{};
    for (int x = static_cast<int>(boundingBox.GetMinX()); static_cast<float>(x) <= boundingBox.GetMaxX(); x++)
    {
        for (int y = static_cast<int>(boundingBox.GetMinY()); static_cast<float>(y) <= boundingBox.GetMaxY(); y++)
        {
            for (int z = static_cast<int>(boundingBox.GetMinZ()); static_cast<float>(z) <= boundingBox.GetMaxZ(); z++)
            {
                const EBlockType block = GetBlockTypeAt(x, y, z);
                if (block == EBlockType::Air)
                {
                    continue;
                }
                BoundingBox helper = BlockTypeList::GetBlockTypeData(block)->GetBoundingBox();
                helper.Move(static_cast<float>(x), static_cast<float>(y), static_cast<float>(z));
                result.push_back(helper);
            }
        }
    }
    return result;
}

void SinglePlayerWorld::HandleMouseMovementInput(const double x, const double y)
{
    PlayerController.HandleMouseMovementInput(static_cast<float>(x), static_cast<float>(y));
}

void SinglePlayerWorld::HandleKeyboardPlayerInput(const int key, const int action)
{
    PlayerController.HandleKeyboardMovementInput(key, action);
}

SinglePlayerWorld::~SinglePlayerWorld()
{
    try
    {
        SaveWorld();
    }
    catch (...)
    {
        std::cout << "Error while saving world" << std::endl;
    }
}
