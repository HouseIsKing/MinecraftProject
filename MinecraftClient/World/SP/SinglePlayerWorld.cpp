#include "SinglePlayerWorld.h"
#include "Entities/Generic/ParticleEntity.h"
#include "Entities/Zombie.h"
#include "Util/CustomFileManager.h"
#include "Util/EngineDefaults.h"
#include "Util/PerlinNoise.h"
#include <filesystem>
#include <iostream>


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
        ChunkCoords<SinglePlayerWorld> coords;
        fileManager >> coords;
        int x = coords.X * Chunk<SinglePlayerWorld>::CHUNK_WIDTH;
        int y = coords.Y * Chunk<SinglePlayerWorld>::CHUNK_HEIGHT;
        int z = coords.Z * Chunk<SinglePlayerWorld>::CHUNK_DEPTH;
        fileManager >> Chunks.emplace(std::piecewise_construct, std::forward_as_tuple(x, y, z), std::forward_as_tuple(x, y, z)).first->second;
    }
}

SinglePlayerWorld::SinglePlayerWorld(const uint16_t width, const uint16_t height, const uint16_t depth, GLFWwindow* window) : World(window), LevelWidth(width), LevelHeight(height), LevelDepth(depth)
{
    Entity<SinglePlayerWorld>::SetWorld(this);
    Chunk<SinglePlayerWorld>::SetWorld(this);
    Gui<SinglePlayerWorld>::SetWorld(this);
    for (uint16_t i = 0xFFFF; i != 0; i--)
    {
        EntityAvailableIDs.push(i);
    }
    EntityAvailableIDs.push(0);
    Player = new PlayerController(EngineDefaults::GetNext(width), static_cast<float>(height + 3), EngineDefaults::GetNext(depth));
    Init();
    for (uint16_t i = 1; i <= 10; i++)
    {
        new Zombie(EngineDefaults::GetNext(width), static_cast<float>(LevelHeight + 3), EngineDefaults::GetNext(depth));
    }
}

uint16_t SinglePlayerWorld::RegisterEntity(Entity<SinglePlayerWorld>* entity)
{
    uint16_t index = EntityAvailableIDs.top();
    EntityAvailableIDs.pop();
    Entities.emplace(index, entity);
    return index;
}

void SinglePlayerWorld::RemoveEntity(const uint16_t id)
{
    EntitiesToRemove.push_back(id);
}

void SinglePlayerWorld::Tick()
{
    WorldTime++;
    Shader::SetUnsignedInt(EngineDefaults::GetShader()->GetUniformInt("worldTime"), static_cast<GLuint>(WorldTime % 24000L));
    for (uint16_t val : EntitiesToRemove)
    {
        Entities.erase(val);
        EntityAvailableIDs.push(val);
    }
    EntitiesToRemove.clear();
    for (const auto& val : Entities | std::views::values)
    {
        val->Tick();
    }
    const int numTilesToTick = LevelWidth * LevelHeight * LevelDepth / 400;
    for (int i = 0; i < numTilesToTick; i++)
    {
        const int x = EngineDefaults::GetNext(LevelWidth);
        const int y = EngineDefaults::GetNext(LevelHeight);
        const int z = EngineDefaults::GetNext(LevelDepth);
        GetBlockAt(x, y, z)->Tick(this, x, y, z);
    }
}

void SinglePlayerWorld::Init()
{
    World::Init();
    if (std::filesystem::exists("level.dat"))
    {
        LoadWorld();
    }
    else
    {
        GenerateLevel();
    }
    RecalculateLightLevels();
}

void SinglePlayerWorld::RecalculateLightLevels()
{
    LightLevels.clear();
    LightLevels.reserve(static_cast<size_t>(LevelWidth * LevelDepth));
    for (int i = 0; i < LevelWidth; i++)
    {
        for (int j = 0; j < LevelDepth; j++)
        {
            LightLevels.emplace(glm::ivec2(i, j), static_cast<uint8_t>(0));
        }
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
    const glm::ivec2 coords{x, z};
    const int previousLightLevel = LightLevels[coords];
    for (int y = LevelHeight - 1; y >= 0; y--)
    {
        if (GetBlockAt(x, y, z)->IsBlockingLight() || y == 0)
        {
            LightLevels[coords] = static_cast<uint8_t>(y);
            return y - previousLightLevel;
        }
    }
    return 0 - previousLightLevel;
}

void SinglePlayerWorld::UpdateChunksNear(const int x, const int y, const int z)
{
    Chunk<SinglePlayerWorld>* chunk = GetChunkAt(x, y + 1, z);
    if (chunk != nullptr)
    {
        AddChunkAsDirty(chunk);
    }
    chunk = GetChunkAt(x, y - 1, z);
    if (chunk != nullptr)
    {
        AddChunkAsDirty(chunk);
    }
    chunk = GetChunkAt(x, y, z + 1);
    if (chunk != nullptr)
    {
        AddChunkAsDirty(chunk);
    }
    chunk = GetChunkAt(x, y, z - 1);
    if (chunk != nullptr)
    {
        AddChunkAsDirty(chunk);
    }
    chunk = GetChunkAt(x + 1, y, z);
    if (chunk != nullptr)
    {
        AddChunkAsDirty(chunk);
    }
    chunk = GetChunkAt(x - 1, y, z);
    if (chunk != nullptr)
    {
        AddChunkAsDirty(chunk);
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
                Chunks.emplace(std::piecewise_construct,
                               std::forward_as_tuple(x * Chunk<SinglePlayerWorld>::CHUNK_WIDTH,
                                                     y * Chunk<SinglePlayerWorld>::CHUNK_HEIGHT,
                                                     z * Chunk<SinglePlayerWorld>::CHUNK_DEPTH),
                               std::forward_as_tuple(x * Chunk<SinglePlayerWorld>::CHUNK_WIDTH,
                                                     y * Chunk<SinglePlayerWorld>::CHUNK_HEIGHT,
                                                     z * Chunk<SinglePlayerWorld>::CHUNK_DEPTH));
            }
        }
    }
}

void SinglePlayerWorld::GenerateLevel()
{
    const auto amountX = static_cast<uint16_t>(std::ceil(static_cast<float>(LevelWidth) / static_cast<float>(Chunk<SinglePlayerWorld>::CHUNK_WIDTH)));
    const auto amountY = static_cast<uint16_t>(std::ceil(static_cast<float>(LevelHeight) / static_cast<float>(Chunk<SinglePlayerWorld>::CHUNK_HEIGHT)));
    const auto amountZ = static_cast<uint16_t>(std::ceil(static_cast<float>(LevelDepth) / static_cast<float>(Chunk<SinglePlayerWorld>::CHUNK_DEPTH)));
    GenerateChunks(amountX, amountY, amountZ);
    const std::vector<int> firstHeightMap = PerlinNoise(0).Generate(LevelWidth, LevelDepth);
    const std::vector<int> secondHeightMap = PerlinNoise(0).Generate(LevelWidth, LevelDepth);
    const std::vector<int> cliffMap = PerlinNoise(1).Generate(LevelWidth, LevelDepth);
    const std::vector<int> rockMap = PerlinNoise(1).Generate(LevelWidth, LevelDepth);

    for (int x = 0; x < LevelWidth; x++)
    {
        for (int y = 0; y < LevelHeight; y++)
        {
            for (int z = 0; z < LevelDepth; z++)
            {
                int firstHeightValue = firstHeightMap[static_cast<size_t>(x + z * LevelWidth)];
                int secondHeightValue = secondHeightMap[static_cast<size_t>(x + z * LevelWidth)];

                if (cliffMap[static_cast<size_t>(x + z * LevelWidth)] < 128)
                {
                    secondHeightValue = firstHeightValue;
                }

                const int maxLevelHeight = std::max(secondHeightValue, firstHeightValue) / 8 + LevelHeight / 3;
                int maxRockHeight = rockMap[static_cast<size_t>(x + z * LevelWidth)] / 8 + LevelHeight / 3;

                // Keep it below the max height of the level
                if (maxRockHeight > maxLevelHeight - 2)
                {
                    maxRockHeight = maxLevelHeight - 2;
                }

                auto blockType = EBlockType::Air;
                // Grass layer
                if (y == maxLevelHeight)
                {
                    blockType = EBlockType::Grass;
                }

                // Dirt layer
                if (y < maxLevelHeight)
                {
                    blockType = EBlockType::Dirt;
                }

                // Rock layer
                if (y <= maxRockHeight)
                {
                    blockType = EBlockType::Stone;
                }

                GetChunkAt(x, y, z)->SetBlockTypeAt(x, y, z, blockType);
            }
        }
    }
    const int count = LevelWidth * LevelHeight * LevelDepth / 256 / 64;
    for (int i = 0; i < count; i++)
    {
        float x = EngineDefaults::GetNextFloat() * static_cast<float>(LevelWidth);
        float y = EngineDefaults::GetNextFloat() * static_cast<float>(LevelHeight);
        float z = EngineDefaults::GetNextFloat() * static_cast<float>(LevelDepth);
        const int length = static_cast<int>(EngineDefaults::GetNextFloat() + EngineDefaults::GetNextFloat() * 150.0F);
        float dir1 = EngineDefaults::GetNextFloat() * glm::pi<float>() * 2.0F;
        float dir1Change = 0.0F;
        float dir2 = EngineDefaults::GetNextFloat() * glm::pi<float>() * 2.0F;
        float dir2Change = 0.0F;
        for (int l = 0; l < length; l++)
        {
            x += sin(dir1) * cos(dir2);
            z += cos(dir1) * cos(dir2);
            y += sin(dir2);
            dir1 += dir1Change * 0.2F;
            dir1Change *= 0.9F;
            dir1Change += EngineDefaults::GetNextFloat() - EngineDefaults::GetNextFloat();
            dir2 += dir2Change * 0.5F;
            dir2 *= 0.5F;
            dir2Change *= 0.9F;
            dir2Change += EngineDefaults::GetNextFloat() - EngineDefaults::GetNextFloat();
            const float size = sin(static_cast<float>(l) * glm::pi<float>() / static_cast<float>(length)) * 2.5F + 1.0F;
            for (int xx = static_cast<int>(x - size); xx <= static_cast<int>(x + size); xx++)
            {
                for (int yy = static_cast<int>(y - size); yy <= static_cast<int>(y + size); yy++)
                {
                    for (int zz = static_cast<int>(z - size); zz <= static_cast<int>(z + size); zz++)
                    {
                        const float deltaX = static_cast<float>(xx) - x;
                        const float deltaY = static_cast<float>(yy) - y;
                        const float deltaZ = static_cast<float>(zz) - z;
                        if (const float distance = deltaX * deltaX + deltaY * deltaY * 2.0F + deltaZ * deltaZ; distance < size * size && xx >= 1 && yy >= 1 && zz >= 1 && xx < LevelWidth - 1 && yy < LevelHeight - 1 && zz < LevelDepth - 1)
                        {
                            if (Chunk<SinglePlayerWorld>* chunk = GetChunkAt(xx, yy, zz); chunk->GetBlockTypeAt(xx, yy, zz) == EBlockType::Stone)
                            {
                                chunk->SetBlockTypeAt(xx, yy, zz, EBlockType::Air);
                            }
                        }
                    }
                }
            }
        }
    }
}

void SinglePlayerWorld::PlaceBlockAt(const int x, const int y, const int z, const EBlockType blockType)
{
    Chunk<SinglePlayerWorld>* chunk = GetChunkAt(x, y, z);
    if (chunk == nullptr)
    {
        return;
    }
    const Block* previousBlock = chunk->GetBlockAt(x, y, z);
    const Block* block = BlockTypeList::GetBlockTypeData(blockType);
    chunk->SetBlockTypeAt(x, y, z, blockType);
    if (block->IsSolidBlock() != previousBlock->IsSolidBlock())
    {
        UpdateChunksNear(x, y, z);
    }
    if (block->IsBlockingLight() != previousBlock->IsBlockingLight())
    {
        const int lightLevelsChange = RecalculateLightLevels(x, z);
        for (int i = 0; i <= abs(lightLevelsChange); i++)
        {
            if (Chunk<SinglePlayerWorld>* chunkLight = GetChunkAt(x, y + i * (lightLevelsChange > 0 ? -1 : 1), z); chunkLight != nullptr)
            {
                AddChunkAsDirty(chunkLight);
            }
        }
    }
}

void SinglePlayerWorld::RemoveBlockAt(const int x, const int y, const int z)
{
    Chunk<SinglePlayerWorld>* chunk = GetChunkAt(x, y, z);
    if (chunk == nullptr)
    {
        return;
    }
    const Block* block = chunk->GetBlockAt(x, y, z);
    block->OnBreak(this, x, y, z);
    chunk->SetBlockTypeAt(x, y, z, EBlockType::Air);
    if (block->IsSolidBlock())
    {
        UpdateChunksNear(x, y, z);
    }
    if (block->IsBlockingLight())
    {
        const int lightLevelsChange = RecalculateLightLevels(x, z);
        for (int i = 0; i <= abs(lightLevelsChange); i++)
        {
            if (Chunk<SinglePlayerWorld>* chunkLight = GetChunkAt(x, y + i * (lightLevelsChange > 0 ? 1 : -1), z); chunkLight != nullptr)
            {
                AddChunkAsDirty(chunkLight);
            }
        }
    }
}

void SinglePlayerWorld::Run()
{
    int i;
    for (i = 0; i < static_cast<int>(TickTimer / TICK_RATE); i++)
    {
        Tick();
    }
    TickTimer -= static_cast<float>(i) * TICK_RATE;
    DrawWorld(TickTimer / TICK_RATE);
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
    glDeleteBuffers(1, &FogsBuffer);
}
