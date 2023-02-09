#include "World.h"
//#include "Entities/Zombie.h"
#include "Util/CustomFileManager.h"
#include "Util/PerlinNoise.h"
#include <filesystem>
#include <glm/ext/scalar_constants.hpp>
#include <iostream>

#include "Blocks/BlockTypeList.h"

World* World::TheWorld = nullptr;

void World::SaveWorld() const
{
    CustomFileManager fileManager{"level.dat", "w"};
    const WorldState& state = State.GetState();
    fileManager << state.Chunks.size();
    for (const auto& val : state.Chunks | std::views::values)
    {
        fileManager << val;
    }
}

void World::LoadWorld()
{
    if (std::filesystem::exists("level.dat"))
    {
        CustomFileManager fileManager{"level.dat", "r"};
        size_t size = 0;
        fileManager >> size;
        for (size_t i = 0; i < size; i++)
        {
            ChunkCoords coords;
            fileManager >> coords;
            const int x = coords.GetX() * EngineDefaults::CHUNK_WIDTH;
            const int y = coords.GetY() * EngineDefaults::CHUNK_HEIGHT;
            const int z = coords.GetZ() * EngineDefaults::CHUNK_DEPTH;
            State.AddChunk(x, y, z);
            fileManager >> State.GetChunk(coords);
        }
    }
    else
    {
        GenerateLevel();
    }
    RecalculateLightLevels();
    /*
    for (int i = 0; i < 10; i++)
    {
        new Zombie(RandomEngineState.GetNext(width), static_cast<float>(LevelHeight + 3), RandomEngineState.GetNext(depth));
    }
    */
    State.ClearAllChanges();
}

World::World(const uint16_t width, const uint16_t height, const uint16_t depth) : LevelWidth(width), LevelHeight(height), LevelDepth(depth)
{
    TheWorld = this;
    for (uint16_t i = 0xFFFF; i != 0; i--)
    {
        EntityAvailableIDs.push(i);
    }
    EntityAvailableIDs.push(0);
    BlockTypeList::InitBlockTypes();
}

void World::RemovePlayer(const uint16_t id)
{
    PlayersToRemove.push_back(id);
}

Player& World::GetPlayer(const uint16_t id)
{
    return State.GetPlayer(id);
}

void World::Tick()
{
    State.SetWorldTime(State.GetState().WorldTime + 1);
    TickRandomEngine = State.GetState().RandomEngine;
    for (auto& val : PlayersToAdd)
    {
        State.AddPlayer(val.EntityId, val.EntityTransform.Position.x, val.EntityTransform.Position.y, val.EntityTransform.Position.z);
    }
    PlayersToAdd.clear();
    for (uint16_t val : PlayersToRemove)
    {
        State.RemovePlayer(val);
        EntityAvailableIDs.push(val);
    }
    PlayersToRemove.clear();
    for (auto it = State.GetPlayersIterator(); it != State.GetState().Players.end(); ++it)
    {
        it->second.Tick();
    }
    const int numTilesToTick = LevelWidth * LevelHeight * LevelDepth / 400;
    for (int i = 0; i < numTilesToTick; i++)
    {
        const int x = TickRandomEngine.GetNext(LevelWidth);
        const int y = TickRandomEngine.GetNext(LevelHeight);
        const int z = TickRandomEngine.GetNext(LevelDepth);
        GetBlockAt(x, y, z)->Tick(this, x, y, z);
    }
    State.SetRandomSeed(TickRandomEngine.Seed);
    BuildWorldChanges();
}

void World::RecalculateLightLevels()
{
    for (int x = 0; x < LevelWidth; x++)
    {
        for (int z = 0; z < LevelDepth; z++)
        {
            State.AddLight(glm::ivec2(x, z), static_cast<uint8_t>(0));
            RecalculateLightLevels(x, z);
        }
    }
}

int16_t World::RecalculateLightLevels(const int x, const int z)
{
    if (const auto pos = glm::ivec2(x, z); State.GetState().Lights.contains(pos))
    {
        const uint8_t previousLightLevel = State.GetState().Lights.at(pos);
        for (auto y = static_cast<uint8_t>(LevelHeight - 1); y > 0; y--)
        {
            if (GetBlockAt(x, y, z)->IsBlockingLight())
            {
                State.ChangeLight(pos, y);
                return static_cast<int16_t>(y - previousLightLevel);
            }
        }
        State.ChangeLight(pos, 0);
        return static_cast<int16_t>(-previousLightLevel);
    }
    return 0;
}

void World::BuildWorldChanges()
{
    std::vector<uint8_t>& changesThisTick = ChangesLists[State.GetState().WorldTime % ChangesLists.size()];
    changesThisTick.clear();
    State.WriteChangesToVector(changesThisTick);
}

bool World::RevertWorldState(const uint64_t tick)
{
    if (tick + ChangesLists.size() <= State.GetState().WorldTime || tick > State.GetState().WorldTime)
    {
        return false;
    }
    for (size_t i = State.GetState().WorldTime; i > tick; i--)
    {
        RevertChangesList(ChangesLists[i % ChangesLists.size()]);
    }
    return true;
}

void World::SimulateTicks(const uint8_t tickCount)
{
    for (uint8_t i = 0; i < tickCount; i++)
    {
        Tick();
    }
}

void World::RevertChangesList(const std::vector<uint8_t>& changes)
{
    size_t pos = 0;
    while (pos < changes.size())
    {
        switch (EngineDefaults::ReadDataFromVector<EChangeType>(changes, pos))
        {
        case EChangeType::ChunkState:
            {
                State.GetChunk(EngineDefaults::ReadDataFromVector<ChunkCoords>(changes, pos)).RevertChunkChanges(changes, pos);
                break;
            }
        case EChangeType::PlayerState:
            {
                State.GetPlayer(EngineDefaults::ReadDataFromVector<uint16_t>(changes, pos)).RevertEntityChanges(changes, pos);
                break;
            }
        case EChangeType::LightState:
            {
                const uint32_t count = EngineDefaults::ReadDataFromVector<uint32_t>(changes, pos);
                for (uint32_t i = 0; i < count; i++)
                {
                    const glm::ivec2 lightPos = EngineDefaults::ReadDataFromVector<glm::ivec2>(changes, pos);
                    const uint8_t lightLevel = EngineDefaults::ReadDataFromVector<uint8_t>(changes, pos);
                    pos += sizeof(uint8_t);
                    State.ChangeLight(lightPos, lightLevel);
                }
                break;
            }
        case EChangeType::RandomState:
            {
                State.SetRandomSeed(EngineDefaults::ReadDataFromVector<uint64_t>(changes, pos));
                pos += sizeof(uint64_t);
                break;
            }
        case EChangeType::WorldTime:
            {
                State.SetWorldTime(EngineDefaults::ReadDataFromVector<uint64_t>(changes, pos));
                pos += sizeof(uint64_t);
                break;
            }
        case EChangeType::ChunkEnterWorld:
            {
                const uint32_t changeCount = EngineDefaults::ReadDataFromVector<uint32_t>(changes, pos);
                for (uint32_t i = 0; i < changeCount; i++)
                {
                    State.RemoveChunk(EngineDefaults::ReadDataFromVector<ChunkState>(changes, pos).ChunkPosition);
                }
                break;
            }
        case EChangeType::ChunkLeaveWorld:
            {
                const uint32_t changeCount = EngineDefaults::ReadDataFromVector<uint32_t>(changes, pos);
                for (uint32_t i = 0; i < changeCount; i++)
                {
                    State.AddChunk(EngineDefaults::ReadDataFromVector<ChunkState>(changes, pos));
                }
                break;
            }
        case EChangeType::PlayerEnterWorld:
            {
                const uint32_t changeCount = EngineDefaults::ReadDataFromVector<uint32_t>(changes, pos);
                for (uint32_t i = 0; i < changeCount; i++)
                {
                    State.RemovePlayer(EngineDefaults::ReadDataFromVector<PlayerState>(changes, pos).EntityId);
                }
                break;
            }
        case EChangeType::PlayerLeaveWorld:
            {
                const uint32_t changeCount = EngineDefaults::ReadDataFromVector<uint32_t>(changes, pos);
                for (uint32_t i = 0; i < changeCount; i++)
                {
                    State.AddPlayer(EngineDefaults::ReadDataFromVector<PlayerState>(changes, pos));
                }
                break;
            }
        case EChangeType::LightEnterWorld:
            {
                const uint32_t changeCount = EngineDefaults::ReadDataFromVector<uint32_t>(changes, pos);
                for (uint32_t i = 0; i < changeCount; i++)
                {
                    const auto& lightPos = EngineDefaults::ReadDataFromVector<glm::ivec2>(changes, pos);
                    pos += sizeof uint8_t;
                    State.RemoveLight(lightPos);
                }
                break;
            }
        case EChangeType::LightLeaveWorld:
            {
                const uint32_t changeCount = EngineDefaults::ReadDataFromVector<uint32_t>(changes, pos);
                for (uint32_t i = 0; i < changeCount; i++)
                {
                    const auto& lightPos = EngineDefaults::ReadDataFromVector<glm::ivec2>(changes, pos);
                    const uint8_t lightLevel = EngineDefaults::ReadDataFromVector<uint8_t>(changes, pos);
                    State.AddLight(lightPos, lightLevel);
                }
                break;
            }
        }
    }
    State.ClearAllChanges();
}

void World::GenerateChunks(const uint16_t amountX, const uint16_t amountY, const uint16_t amountZ)
{
    for (int x = 0; x < amountX; x++)
    {
        for (int y = 0; y < amountY; y++)
        {
            for (int z = 0; z < amountZ; z++)
            {
                State.AddChunk(x * EngineDefaults::CHUNK_WIDTH, y * EngineDefaults::CHUNK_HEIGHT, z * EngineDefaults::CHUNK_DEPTH);
            }
        }
    }
}

void World::GenerateLevel()
{
    const auto amountX = static_cast<uint16_t>(std::ceil(static_cast<float>(LevelWidth) / static_cast<float>(EngineDefaults::CHUNK_WIDTH)));
    const auto amountY = static_cast<uint16_t>(std::ceil(static_cast<float>(LevelHeight) / static_cast<float>(EngineDefaults::CHUNK_HEIGHT)));
    const auto amountZ = static_cast<uint16_t>(std::ceil(static_cast<float>(LevelDepth) / static_cast<float>(EngineDefaults::CHUNK_DEPTH)));
    GenerateChunks(amountX, amountY, amountZ);
    const std::vector<int> firstHeightMap = PerlinNoise(0).Generate(LevelWidth, LevelDepth);
    const std::vector<int> secondHeightMap = PerlinNoise(0).Generate(LevelWidth, LevelDepth);
    const std::vector<int> cliffMap = PerlinNoise(1).Generate(LevelWidth, LevelDepth);
    const std::vector<int> rockMap = PerlinNoise(1).Generate(LevelWidth, LevelDepth);
    CustomRandomEngine random = State.GetState().RandomEngine;

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
        float x = random.GetNextFloat() * static_cast<float>(LevelWidth);
        float y = random.GetNextFloat() * static_cast<float>(LevelHeight);
        float z = random.GetNextFloat() * static_cast<float>(LevelDepth);
        const int length = static_cast<int>(random.GetNextFloat() + random.GetNextFloat() * 150.0F);
        float dir1 = random.GetNextFloat() * glm::pi<float>() * 2.0F;
        float dir1Change = 0.0F;
        float dir2 = random.GetNextFloat() * glm::pi<float>() * 2.0F;
        float dir2Change = 0.0F;
        for (int l = 0; l < length; l++)
        {
            x += sin(dir1) * cos(dir2);
            z += cos(dir1) * cos(dir2);
            y += sin(dir2);
            dir1 += dir1Change * 0.2F;
            dir1Change *= 0.9F;
            dir1Change += random.GetNextFloat() - random.GetNextFloat();
            dir2 += dir2Change * 0.5F;
            dir2 *= 0.5F;
            dir2Change *= 0.9F;
            dir2Change += random.GetNextFloat() - random.GetNextFloat();
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
    State.SetRandomSeed(random.Seed);
}

const Block* World::GetBlockAt(const int x, const int y, const int z)
{
    return BlockTypeList::GetBlockTypeData(GetBlockTypeAt(x, y, z));
}

EBlockType World::GetBlockTypeAt(const int x, const int y, const int z)
{
    const Chunk* chunk = GetChunkAt(x, y, z);
    if (chunk == nullptr)
    {
        return EBlockType::Air;
    }
    return chunk->GetBlockTypeAt(x, y, z);
}

Chunk* World::GetChunkAt(const int x, const int y, const int z)
{
    const auto pos = ChunkCoords(x, y, z);
    if (!State.GetState().Chunks.contains(pos))
    {
        return nullptr;
    }
    return &State.GetChunk(pos);
}

/**
 * \brief Rounds the float vector to integer
 * \param pos position to round
 * \return returns brightness at the rounded position
 */
int World::GetBrightnessAt(const glm::vec3 pos) const
{
    return GetBrightnessAt(static_cast<int>(pos.x), static_cast<int>(pos.y), static_cast<int>(pos.z));
}

int World::GetBrightnessAt(const int x, const int y, const int z) const
{
    if (const auto pos = glm::ivec2(x, z); State.GetState().Lights.contains(pos))
    {
        if (y > State.GetState().Lights.at(pos))
        {
            return 1;
        }
    }
    return 0;
}

bool World::IsBlockSolid(const int x, const int y, const int z)
{
    return GetBlockAt(x, y, z)->IsSolidBlock();
}

bool World::IsBlockExists(const int x, const int y, const int z)
{
    return GetBlockTypeAt(x, y, z) != EBlockType::Air;
}

void World::PlaceBlockAt(const int x, const int y, const int z, const EBlockType blockType)
{
    Chunk* chunk = GetChunkAt(x, y, z);
    if (chunk == nullptr)
    {
        return;
    }
    const Block* previousBlock = chunk->GetBlockAt(x, y, z);
    const Block* block = BlockTypeList::GetBlockTypeData(blockType);
    chunk->SetBlockTypeAt(x, y, z, blockType);
    if (block->IsBlockingLight() != previousBlock->IsBlockingLight())
    {
        RecalculateLightLevels(x, z);
    }
}

void World::RemoveBlockAt(const int x, const int y, const int z)
{
    Chunk* chunk = GetChunkAt(x, y, z);
    if (chunk == nullptr)
    {
        return;
    }
    const Block* block = chunk->GetBlockAt(x, y, z);
    block->OnBreak(this, x, y, z);
    chunk->SetBlockTypeAt(x, y, z, EBlockType::Air);
    if (block->IsBlockingLight())
    {
        RecalculateLightLevels(x, z);
    }
}

std::vector<BoundingBox> World::GetBlockBoxesInBoundingBox(const BoundingBox& boundingBox)
{
    std::vector<BoundingBox> result{};
    for (int x = static_cast<int>(boundingBox.GetMinX()); static_cast<float>(x) <= boundingBox.GetMaxX(); x++)
    {
        for (int y = static_cast<int>(boundingBox.GetMinY()); static_cast<float>(y) <= boundingBox.GetMaxY(); y++)
        {
            for (int z = static_cast<int>(boundingBox.GetMinZ()); static_cast<float>(z) <= boundingBox.GetMaxZ(); z++)
            {
                if (const Block* block = GetBlockAt(x, y, z); block->IsSolidBlock())
                {
                    BoundingBox helper = block->GetBoundingBox();
                    helper.Move(static_cast<float>(x), static_cast<float>(y), static_cast<float>(z));
                    result.push_back(helper);
                }
            }
        }
    }
    return result;
}

uint64_t World::GetWorldTime() const
{
    return State.GetState().WorldTime;
}

World::~World()
{
    TheWorld = nullptr;
}

World* World::GetWorld()
{
    return TheWorld;
}
