//
// Created by amit on 4/21/2022.
//

#include "SinglePlayerWorld.h"
#include "../Util/CustomFileManager.h"
#include "../Util/EngineDefaults.h"
#include <cmath>
#include <filesystem>
#include <ranges>

#include "Entities/Zombie.h"

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
    size_t size = 0;
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

SinglePlayerWorld::SinglePlayerWorld(const uint16_t width, const uint16_t height, const uint16_t depth, GLFWwindow* window) : Player(new PlayerController(0, EngineDefaults::GetNext(width), static_cast<float>(height + 3),
    EngineDefaults::GetNext(depth))), LevelWidth(width), LevelHeight(height), LevelDepth(depth), FogsBuffer(0), TheAppWindow(window)
{
    Entity::SetWorld(this);
    Chunk::SetWorld(this);
    Init();

    Entities.emplace(piecewise_construct, forward_as_tuple(static_cast<uint16_t>(0)), forward_as_tuple(dynamic_cast<Entity*>(Player)));
    for (uint16_t i = 1; i <= 100; i++)
    {
        Entities.emplace(piecewise_construct, forward_as_tuple(i), forward_as_tuple(dynamic_cast<Entity*>(new Zombie(i, EngineDefaults::GetNext(width), static_cast<float>(LevelHeight + 3), EngineDefaults::GetNext(depth)))));
    }
    EngineDefaults::BuildTextureUbo();
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
    //Shader::SetVec3(EngineDefaults::GetShader()->GetUniformInt("directionalLightDirection"), -1.0F, -1.0F, -1.0F);
    InitFog();
    BlockTypeList::InitBlockTypes();
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
    RecalculateLightLevels();
}

void SinglePlayerWorld::InitFog()
{
    const vector fogs{14.0F / 255.0F, 11.0F / 255.0F, 10.0F / 255.0F, 1.0F, 0.06F, 0.0F, 0.0F, 0.0F, 254.0F / 255.0F, 251.0F / 255.0F, 250.0F / 255.0F, 1.0F, 0.001F, 0.0F, 0.0F, 0.0F};
    glGenBuffers(1, &FogsBuffer);
    glBindBuffer(GL_UNIFORM_BUFFER, FogsBuffer);
    glBufferData(GL_UNIFORM_BUFFER, static_cast<GLintptr>(fogs.size() * sizeof(float)), fogs.data(), GL_STATIC_COPY);
    glBindBufferRange(GL_UNIFORM_BUFFER, 1, FogsBuffer, 0, static_cast<GLintptr>(fogs.size() * sizeof(float)));
    glUniformBlockBinding(EngineDefaults::GetShader()->Program, EngineDefaults::GetShader()->GetUniformBlockIndex("fogBlock"), 1);
}

void SinglePlayerWorld::RecalculateLightLevels()
{
    LightLevels.clear();
    LightLevels.reserve(static_cast<size_t>(LevelWidth * LevelDepth));
    while (LightLevels.size() < static_cast<size_t>(LevelWidth * LevelDepth))
    {
        LightLevels.push_back(0);
    }
    for (int x = 0; x < LevelWidth; x++)
    {
        for (int z = 0; z < LevelDepth; z++)
        {
            RecalculateLightLevels(x, z);
        }
    }
}

int SinglePlayerWorld::RecalculateLightLevels(const int x, const int z)
{
    const int previousLightLevel = LightLevels[static_cast<size_t>(x * LevelDepth + z)];
    for (int y = LevelHeight - 1; y >= 0; y--)
    {
        if (GetBlockTypeAt(x, y, z) != EBlockType::Air || y == 0)
        {
            LightLevels.at(static_cast<size_t>(x * LevelDepth + z)) = static_cast<uint8_t>(y);
            return y - previousLightLevel;
        }
    }
    return 0 - previousLightLevel;
}

void SinglePlayerWorld::UpdateChunksNear(const int x, const int y, const int z)
{
    Chunk* chunk = GetChunkAt(x, y + 1, z);
    if (chunk != nullptr)
    {
        chunk->IsDirty = true;
    }
    chunk = GetChunkAt(x, y - 1, z);
    if (chunk != nullptr)
    {
        chunk->IsDirty = true;
    }
    chunk = GetChunkAt(x, y, z + 1);
    if (chunk != nullptr)
    {
        chunk->IsDirty = true;
    }
    chunk = GetChunkAt(x, y, z - 1);
    if (chunk != nullptr)
    {
        chunk->IsDirty = true;
    }
    chunk = GetChunkAt(x + 1, y, z);
    if (chunk != nullptr)
    {
        chunk->IsDirty = true;
    }
    chunk = GetChunkAt(x - 1, y, z);
    if (chunk != nullptr)
    {
        chunk->IsDirty = true;
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
                            if (y + j * Chunk::CHUNK_HEIGHT >= LevelHeight - 7)
                            {
                                chunk->SetBlockTypeAt(x, y, z, EBlockType::Grass);
                            }
                            else
                            {
                                chunk->SetBlockTypeAt(x, y, z, EBlockType::Cobblestone);
                            }
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

int SinglePlayerWorld::GetBrightnessAt(const int x, const int y, const int z) const
{
    if (x < 0 || x >= LevelWidth || y < 0 || y >= LevelHeight || z < 0 || z >= LevelDepth)
    {
        return 1;
    }
    if (const uint8_t lightLevel = LightLevels.at(static_cast<size_t>(x * LevelDepth + z)); y >= lightLevel)
    {
        return 1;
    }
    return 0;
}

bool SinglePlayerWorld::IsBlockExists(const int x, const int y, const int z)
{
    return GetBlockTypeAt(x, y, z) != EBlockType::Air;
}

void SinglePlayerWorld::PlaceBlockAt(const int x, const int y, const int z)
{
    Chunk* chunk = GetChunkAt(x, y, z);
    if (chunk == nullptr)
    {
        return;
    }
    if (y >= LevelHeight - 7)
    {
        chunk->SetBlockTypeAt(x, y, z, EBlockType::Grass);
    }
    else
    {
        chunk->SetBlockTypeAt(x, y, z, EBlockType::Cobblestone);
    }
    UpdateChunksNear(x, y, z);
    const int lightLevelsChange = RecalculateLightLevels(x, z);
    for (int i = 0; i <= abs(lightLevelsChange); i++)
    {
        if (Chunk* chunkLight = GetChunkAt(x, y + i * (lightLevelsChange > 0 ? -1 : 1), z); chunkLight != nullptr)
        {
            chunkLight->IsDirty = true;
        }
    }
}

void SinglePlayerWorld::RemoveBlockAt(const int x, const int y, const int z)
{
    Chunk* chunk = GetChunkAt(x, y, z);
    if (chunk == nullptr)
    {
        return;
    }
    chunk->SetBlockTypeAt(x, y, z, EBlockType::Air);
    UpdateChunksNear(x, y, z);
    const int lightLevelsChange = RecalculateLightLevels(x, z);
    for (int i = 0; i <= abs(lightLevelsChange); i++)
    {
        if (Chunk* chunkLight = GetChunkAt(x, y + i * (lightLevelsChange > 0 ? 1 : -1), z); chunkLight != nullptr)
        {
            chunkLight->IsDirty = true;
        }
    }
}

void SinglePlayerWorld::DrawWorld()
{
    glEnable(GL_CULL_FACE);
    uint8_t chunksRebuilt = 0;
    for (auto& [fst, snd] : Chunks)
    {
        if (chunksRebuilt < MaxChunkRebuilt && snd.IsDirty)
        {
            chunksRebuilt++;
            snd.GenerateTessellationData();
        }
        if (ChunkCoords pos = fst; Player->GetCameraFrustum().CubeInFrustum(static_cast<float>(pos.GetX() * Chunk::CHUNK_WIDTH), static_cast<float>(pos.GetY() * Chunk::CHUNK_HEIGHT), static_cast<float>(pos.GetZ() * Chunk::CHUNK_DEPTH), static_cast<float>(pos.GetX() * Chunk::CHUNK_WIDTH + Chunk::CHUNK_WIDTH), static_cast<float>(pos.GetY() * Chunk::CHUNK_HEIGHT + Chunk::CHUNK_HEIGHT), static_cast<float>(pos.GetZ() * Chunk::CHUNK_DEPTH + Chunk::CHUNK_DEPTH)))
        {
            snd.Draw();
        }
    }
    for (const auto& entity : Entities | std::views::values)
    {
        if (Player->GetCameraFrustum().CubeInFrustum(entity->GetBoundingBox()))
        {
            entity->DoRender();
        }
    }
    glDisable(GL_CULL_FACE);
}

GLFWwindow* SinglePlayerWorld::GetWindow() const
{
    return TheAppWindow;
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
