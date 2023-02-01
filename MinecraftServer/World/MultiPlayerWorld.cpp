#include "MultiPlayerWorld.h"
#include "Entities/Zombie.h"
#include "Util/EngineDefaults.h"
#include "Util/PerlinNoise.h"
#include <GLFW/glfw3.h>
#include <filesystem>
#include <glm/ext/scalar_constants.hpp>
#include <iostream>

#include "Network/Packets/ClientInputPacket.h"
#include "Util/States/PlayerState.h"


void MultiPlayerWorld::SaveWorld()
{
    CustomFileManager fileManager{"level.dat", "w"};
    fileManager << Chunks.size();
    for (auto& val : Chunks | std::views::values)
    {
        fileManager << val;
    }
}

void MultiPlayerWorld::LoadWorld()
{
    CustomFileManager fileManager{"level.dat", "r"};
    size_t size = 0;
    fileManager >> size;
    for (size_t i = 0; i < size; i++)
    {
        ChunkCoords coords;
        fileManager >> coords;
        int x = coords.GetX() * EngineDefaults::CHUNK_WIDTH;
        int y = coords.GetY() * EngineDefaults::CHUNK_HEIGHT;
        int z = coords.GetZ() * EngineDefaults::CHUNK_DEPTH;
        Chunk& chunk = Chunks.emplace(std::piecewise_construct, std::forward_as_tuple(x, y, z), std::forward_as_tuple(x, y, z)).first->second;
        fileManager >> chunk;
    }
}

MultiPlayerWorld::MultiPlayerWorld(const uint16_t width, const uint16_t height, const uint16_t depth) : LevelWidth(width), LevelHeight(height), LevelDepth(depth)
{
    Entity::SetWorld(this);
    Chunk::SetWorld(this);
    for (uint16_t i = 0xFFFF; i != 0; i--)
    {
        EntityAvailableIDs.push(i);
    }
    EntityAvailableIDs.push(0);
    Init();
    /*
    for (int i = 0; i < 10; i++)
    {
        new Zombie(RandomEngineState.GetNext(width), static_cast<float>(LevelHeight + 3), RandomEngineState.GetNext(depth));
    }
    */
    BuildWorldState();
    NetworkManager.Start();
}

uint16_t MultiPlayerWorld::RegisterEntity(Entity* entity)
{
    const uint16_t index = EntityAvailableIDs.top();
    EntityAvailableIDs.pop();
    EntitiesAdded.push_back(entity);
    return index;
}

void MultiPlayerWorld::RemoveEntity(const uint16_t id)
{
    EntitiesToRemove.push_back(id);
}

void MultiPlayerWorld::Tick()
{
    WorldTime++;
    MaxWorldTime = std::max(MaxWorldTime, WorldTime);
    for (auto* val : EntitiesAdded)
    {
        Entities.emplace(val->GetEntityId(), val);
    }
    EntitiesAdded.clear();
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
        const int x = RandomEngineState.GetNext(LevelWidth);
        const int y = RandomEngineState.GetNext(LevelHeight);
        const int z = RandomEngineState.GetNext(LevelDepth);
        GetBlockAt(x, y, z)->Tick(this, x, y, z);
    }
    BuildWorldState();
}

void MultiPlayerWorld::Run()
{
    //Handle newly connected clients
    std::shared_ptr<ConnectionToClient> newCon = NetworkManager.GetNextNewConnection();
    bool needRebuild = false;
    while (newCon != nullptr)
    {
        //CustomRandomEngine random;
        //const auto x = static_cast<float>(random.GetNext(LevelWidth));
        //const auto y = static_cast<float>(LevelHeight + 3);
        //const auto z = static_cast<float>(random.GetNext(LevelDepth));
        Connections.emplace(newCon, new Player{0.0F, 70.0F, 0.0F});
        Entities.emplace(Connections[newCon]->GetEntityId(), Connections[newCon]);
        EntitiesAdded.pop_back();
        uint16_t entityId = Connections[newCon]->GetEntityId();
        auto packet = std::make_shared<Packet>(PacketHeader::PLAYER_ID_PACKET);
        *packet << entityId;
        newCon->WritePacket(packet);
        newCon = NetworkManager.GetNextNewConnection();
        needRebuild = true;
    }
    if (needRebuild)
    {
        BuildWorldState();
    }
    //Handle packets
    std::shared_ptr<PacketData> nextPacket = NetworkManager.GetNextPacket();
    while (nextPacket != nullptr)
    {
        HandlePacket(nextPacket.get());
        nextPacket = NetworkManager.GetNextPacket();
    }
    //Handle disconnected clients
    std::shared_ptr<ConnectionToClient> closedCon = NetworkManager.GetNextRemovedConnection();
    while (closedCon != nullptr)
    {
        RemoveEntity(Connections[closedCon]->GetEntityId());
        Connections.erase(closedCon);
        closedCon = NetworkManager.GetNextRemovedConnection();
    }
}

void MultiPlayerWorld::Init()
{
    BlockTypeList::InitBlockTypes();
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

void MultiPlayerWorld::RecalculateLightLevels()
{
    LightLevels.clear();
    for (int x = 0; x < LevelWidth; x++)
    {
        for (int z = 0; z < LevelDepth; z++)
        {
            LightLevels.emplace(glm::ivec2(x, z), static_cast<uint8_t>(0));
            RecalculateLightLevels(x, z);
        }
    }
}

int MultiPlayerWorld::RecalculateLightLevels(const int x, const int z)
{
    if (const auto pos = glm::ivec2(x, z); LightLevels.contains(pos))
    {
        const int previousLightLevel = LightLevels[pos];
        for (int y = LevelHeight - 1; y >= 0; y--)
        {
            if (GetBlockAt(x, y, z)->IsBlockingLight() || y == 0)
            {
                LightLevels[pos] = static_cast<uint8_t>(y);
                return y - previousLightLevel;
            }
        }
        return 0 - previousLightLevel;
    }
    return 0;
}

void MultiPlayerWorld::BuildWorldState()
{
    WorldState& worldState = WorldStates[WorldTime % WorldStates.size()];
    worldState.Chunks.clear();
    for (Chunk& chunk : Chunks | std::views::values)
    {
        worldState.Chunks.emplace_hint(worldState.Chunks.end(), chunk.GetChunkState().ChunkPosition, chunk.GetChunkState());
    }
    worldState.Entities.clear();
    for (const std::unique_ptr<Entity>& entity : Entities | std::views::values)
    {
        worldState.Entities.emplace_hint(worldState.Entities.end(), entity->GetEntityId(), entity->GetEntityState());
    }
    worldState.Lights.clear();
    for (const auto& [pos, level] : LightLevels)
    {
        worldState.Lights.emplace_hint(worldState.Lights.end(), std::piecewise_construct, std::forward_as_tuple(static_cast<uint16_t>(pos.x), static_cast<uint16_t>(pos.y)), std::forward_as_tuple(static_cast<uint16_t>(pos.x), static_cast<uint16_t>(pos.y), static_cast<uint8_t>(level)));
    }
    worldState.WorldTime = WorldTime;
    worldState.RandomEngine = RandomEngineState;
}

bool MultiPlayerWorld::RevertWorldState(const uint64_t tick)
{
    if (tick <= WorldTime - WorldStates.size() || tick > WorldTime)
    {
        return false;
    }
    const WorldState& prevWorldState = WorldStates[tick % WorldStates.size()];
    const WorldState& currentWorldState = WorldStates[WorldTime % WorldStates.size()];
    const std::vector<uint8_t> changes = currentWorldState - prevWorldState;
    ApplyChangesList(changes);
    return true;
}

void MultiPlayerWorld::SimulateTicks(const uint8_t tickCount)
{
    for (uint8_t i = 0; i < tickCount; i++)
    {
        Tick();
    }
}

void MultiPlayerWorld::ApplyChangesList(const std::vector<uint8_t>& changes)
{
    size_t pos = 0;
    while (pos < changes.size())
    {
        const auto changeType = static_cast<EChangeType>(changes[pos]);
        pos += sizeof(EChangeType);
        switch (changeType)
        {
        case EChangeType::ChunkState:
            {
                const ChunkCoords coords = *reinterpret_cast<const ChunkCoords*>(&changes[pos]);
                pos += sizeof(ChunkCoords);
                Chunks.at(coords).ApplyChunkChanges(changes, pos);
                break;
            }
        case EChangeType::EntityState:
            {
                const uint16_t entityId = *reinterpret_cast<const uint16_t*>(&changes[pos]);
                pos += sizeof(uint16_t);
                Entities.at(entityId)->ApplyEntityChanges(changes, pos);
                break;
            }
        case EChangeType::LightState:
            {
                const LightState lightState = *reinterpret_cast<const LightState*>(&changes[pos]);
                pos += sizeof(LightState);
                LightLevels[glm::ivec2(lightState.X, lightState.Y)] = lightState.LightValue;
                break;
            }
        case EChangeType::RandomState:
            {
                RandomEngineState.Seed = *reinterpret_cast<const uint64_t*>(&changes[pos]);
                pos += sizeof(uint64_t);
                break;
            }
        case EChangeType::WorldTime:
            {
                WorldTime = *reinterpret_cast<const uint64_t*>(&changes[pos]);
                pos += sizeof(uint64_t);
                break;
            }
        case EChangeType::ChunkEnterWorld:
            {
                const ChunkState& chunkState = *reinterpret_cast<const ChunkState*>(&changes[pos]);
                pos += sizeof(ChunkState);
                Chunks.emplace(chunkState.ChunkPosition, chunkState);
                break;
            }
        case EChangeType::ChunkLeaveWorld:
            {
                const ChunkCoords& chunkCoords = *reinterpret_cast<const ChunkCoords*>(&changes[pos]);
                pos += sizeof(ChunkCoords);
                Chunks.erase(chunkCoords);
                break;
            }
        case EChangeType::EntityEnterWorld:
            {
                switch (const auto* entityState = reinterpret_cast<const EntityState*>(&changes[pos]); entityState->EntityType)
                {
                case EEntityType::Player:
                    {
                        const auto* playerState = reinterpret_cast<const PlayerState*>(entityState);
                        pos += sizeof(PlayerState);
                        Entities.emplace(entityState->EntityId, new Player(*playerState));
                        break;
                    }
                default:
                    break;
                }
                break;
            }
        case EChangeType::EntityLeaveWorld:
            {
                uint16_t entityId = *reinterpret_cast<const uint16_t*>(&changes[pos]);
                pos += sizeof(uint16_t);
                Entities.erase(entityId);
                break;
            }
        case EChangeType::LightEnterWorld:
            {
                LightState lightState = *reinterpret_cast<const LightState*>(&changes[pos]);
                pos += sizeof(LightState);
                LightLevels.emplace(glm::ivec2(lightState.X, lightState.Y), lightState.LightValue);
                break;
            }
        case EChangeType::LightLeaveWorld:
            {
                const uint16_t x = *reinterpret_cast<const uint16_t*>(&changes[pos]);
                pos += sizeof(uint16_t);
                const uint16_t y = *reinterpret_cast<const uint16_t*>(&changes[pos]);
                pos += sizeof(uint16_t);
                LightLevels.erase(glm::ivec2(x, y));
            }
        }
    }
}

void MultiPlayerWorld::GenerateChunks(const uint16_t amountX, const uint16_t amountY, const uint16_t amountZ)
{
    for (int x = 0; x < amountX; x++)
    {
        for (int y = 0; y < amountY; y++)
        {
            for (int z = 0; z < amountZ; z++)
            {
                Chunks.emplace(std::piecewise_construct, std::forward_as_tuple(x * EngineDefaults::CHUNK_WIDTH, y * EngineDefaults::CHUNK_HEIGHT, z * EngineDefaults::CHUNK_DEPTH), std::forward_as_tuple(x * EngineDefaults::CHUNK_WIDTH, y * EngineDefaults::CHUNK_HEIGHT, z * EngineDefaults::CHUNK_DEPTH));
            }
        }
    }
}

void MultiPlayerWorld::GenerateLevel()
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

const Block* MultiPlayerWorld::GetBlockAt(const int x, const int y, const int z)
{
    return BlockTypeList::GetBlockTypeData(GetBlockTypeAt(x, y, z));
}

EBlockType MultiPlayerWorld::GetBlockTypeAt(const int x, const int y, const int z)
{
    const Chunk* chunk = GetChunkAt(x, y, z);
    if (chunk == nullptr)
    {
        return EBlockType::Air;
    }
    return chunk->GetBlockTypeAt(x, y, z);
}

Chunk* MultiPlayerWorld::GetChunkAt(const int x, const int y, const int z)
{
    const auto pos = ChunkCoords(x, y, z);
    if (!Chunks.contains(pos))
    {
        return nullptr;
    }
    return &Chunks.at(pos);
}

/**
 * \brief Rounds the float vector to integer
 * \param pos position to round
 * \return returns brightness at the rounded position
 */
int MultiPlayerWorld::GetBrightnessAt(const glm::vec3 pos) const
{
    return GetBrightnessAt(static_cast<int>(pos.x), static_cast<int>(pos.y), static_cast<int>(pos.z));
}

int MultiPlayerWorld::GetBrightnessAt(const int x, const int y, const int z) const
{
    if (const auto pos = glm::ivec2(x, z); LightLevels.contains(pos))
    {
        if (y > LightLevels.at(pos))
        {
            return 1;
        }
    }
    return 0;
}

bool MultiPlayerWorld::IsBlockSolid(const int x, const int y, const int z)
{
    return GetBlockAt(x, y, z)->IsSolidBlock();
}

bool MultiPlayerWorld::IsBlockExists(const int x, const int y, const int z)
{
    return GetBlockTypeAt(x, y, z) != EBlockType::Air;
}

void MultiPlayerWorld::PlaceBlockAt(const int x, const int y, const int z, const EBlockType blockType)
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

void MultiPlayerWorld::RemoveBlockAt(const int x, const int y, const int z)
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

Entity* MultiPlayerWorld::GetEntity(const uint16_t id) const
{
    return Entities.at(id).get();
}

std::vector<BoundingBox> MultiPlayerWorld::GetBlockBoxesInBoundingBox(const BoundingBox& boundingBox)
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

uint64_t MultiPlayerWorld::GetWorldTime() const
{
    return WorldTime;
}

uint64_t MultiPlayerWorld::GetMaxWorldTime() const
{
    return MaxWorldTime;
}

void MultiPlayerWorld::HandlePacket(const PacketData* packet)
{
    if (packet->GetPacketType() == EPacketType::ClientInput)
    {
        const uint64_t currentWorldTick = WorldTime;
        if (const auto* clientInputPacket = reinterpret_cast<const ClientInputPacket*>(packet); clientInputPacket->GetWorldTickSent() == 0)
        {
            std::vector<uint8_t> changes = WorldState() - WorldStates[WorldTime % WorldStates.size()];
            const auto packetToSend = std::make_shared<Packet>(PacketHeader(EPacketType::WorldData, static_cast<uint32_t>(changes.size()) + sizeof(uint64_t)), changes);
            *packetToSend << WorldTime;
            packet->GetConnectionToClient()->WritePacket(packetToSend);
        }
        else if (RevertWorldState(clientInputPacket->GetWorldTickSent() - 1))
        {
            Connections[packet->GetConnectionToClient()]->SetClientInputOnTick(clientInputPacket->GetWorldTickSent(), clientInputPacket->GetState());
            const WorldState& prevWorldState = WorldStates[WorldTime % WorldStates.size()];
            Tick();
            const WorldState& newState = WorldStates[WorldTime % WorldStates.size()];
            std::vector<uint8_t> changes = prevWorldState - newState;
            const auto packetToSend = std::make_shared<Packet>(PacketHeader(EPacketType::WorldData, static_cast<uint32_t>(changes.size()) + sizeof(uint64_t)), changes);
            *packetToSend << WorldTime;
            packet->GetConnectionToClient()->WritePacket(packetToSend);
            SimulateTicks(static_cast<uint8_t>(currentWorldTick - WorldTime));
        }
        else
        {
            std::cout << "Client is too far behind" << std::endl;
        }
    }
}

MultiPlayerWorld::~MultiPlayerWorld()
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
