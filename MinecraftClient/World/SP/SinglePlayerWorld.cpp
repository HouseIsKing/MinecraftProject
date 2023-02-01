#include "SinglePlayerWorld.h"
#include "Entities/Generic/ParticleEntity.h"
#include "Util/CustomFileManager.h"
#include "Util/EngineDefaults.h"
#include "Util/PerlinNoise.h"
#include <filesystem>
#include <iostream>

#include "Entities/Generic/Zombie.h"


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
        int x = coords.X * EngineDefaults::CHUNK_WIDTH;
        int y = coords.Y * EngineDefaults::CHUNK_HEIGHT;
        int z = coords.Z * EngineDefaults::CHUNK_DEPTH;
        fileManager >> Chunks.emplace(std::piecewise_construct, std::forward_as_tuple(x, y, z), std::forward_as_tuple(x, y, z)).first->second;
    }
}

SinglePlayerWorld::SinglePlayerWorld(const uint16_t width, const uint16_t height, const uint16_t depth, GLFWwindow* window) : World(window, height, width, depth)
{
    for (uint16_t i = 0xFFFF; i != 0; i--)
    {
        EntityAvailableIDs.push(i);
    }
    EntityAvailableIDs.push(0);
    PlayerId = (new PlayerController(RandomEngineState.GetNext(width), static_cast<float>(height + 3), RandomEngineState.GetNext(depth)))->GetEntityId();
    Init();
    for (uint16_t i = 1; i <= 10; i++)
    {
        new Zombie(RandomEngineState.GetNext(width), static_cast<float>(LevelHeight + 3), RandomEngineState.GetNext(depth));
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
            World::RecalculateLightLevels(x, z);
        }
    }
}

void SinglePlayerWorld::GenerateChunks(const uint16_t amountX, const uint16_t amountY, const uint16_t amountZ)
{
    for (int x = 0; x < amountX; x++)
    {
        for (int y = 0; y < amountY; y++)
        {
            for (int z = 0; z < amountZ; z++)
            {
                Chunks.emplace(std::piecewise_construct,
                               std::forward_as_tuple(x * EngineDefaults::CHUNK_WIDTH,
                                                     y * EngineDefaults::CHUNK_HEIGHT,
                                                     z * EngineDefaults::CHUNK_DEPTH),
                               std::forward_as_tuple(x * EngineDefaults::CHUNK_WIDTH,
                                                     y * EngineDefaults::CHUNK_HEIGHT,
                                                     z * EngineDefaults::CHUNK_DEPTH));
            }
        }
    }
}

void SinglePlayerWorld::GenerateLevel()
{
    const auto amountX = static_cast<uint16_t>(std::ceil(static_cast<float>(LevelWidth) / static_cast<float>(EngineDefaults::CHUNK_WIDTH)));
    const auto amountY = static_cast<uint16_t>(std::ceil(static_cast<float>(LevelHeight) / static_cast<float>(EngineDefaults::CHUNK_HEIGHT)));
    const auto amountZ = static_cast<uint16_t>(std::ceil(static_cast<float>(LevelDepth) / static_cast<float>(EngineDefaults::CHUNK_DEPTH)));
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
        float x = RandomEngineState.GetNextFloat() * static_cast<float>(LevelWidth);
        float y = RandomEngineState.GetNextFloat() * static_cast<float>(LevelHeight);
        float z = RandomEngineState.GetNextFloat() * static_cast<float>(LevelDepth);
        const int length = static_cast<int>(RandomEngineState.GetNextFloat() + RandomEngineState.GetNextFloat() * 150.0F);
        float dir1 = RandomEngineState.GetNextFloat() * glm::pi<float>() * 2.0F;
        float dir1Change = 0.0F;
        float dir2 = RandomEngineState.GetNextFloat() * glm::pi<float>() * 2.0F;
        float dir2Change = 0.0F;
        for (int l = 0; l < length; l++)
        {
            x += sin(dir1) * cos(dir2);
            z += cos(dir1) * cos(dir2);
            y += sin(dir2);
            dir1 += dir1Change * 0.2F;
            dir1Change *= 0.9F;
            dir1Change += RandomEngineState.GetNextFloat() - RandomEngineState.GetNextFloat();
            dir2 += dir2Change * 0.5F;
            dir2 *= 0.5F;
            dir2Change *= 0.9F;
            dir2Change += RandomEngineState.GetNextFloat() - RandomEngineState.GetNextFloat();
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
                            if (Chunk* chunk = GetChunkAt(xx, yy, zz); chunk->GetBlockTypeAt(xx, yy, zz) == EBlockType::Stone)
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

void SinglePlayerWorld::Run()
{
    int i;
    for (i = 0; i < static_cast<int>(TickTimer / EngineDefaults::TICK_RATE); i++)
    {
        Tick();
    }
    TickTimer -= static_cast<float>(i) * EngineDefaults::TICK_RATE;
    DrawWorld(TickTimer / EngineDefaults::TICK_RATE);
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
