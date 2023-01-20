#include "MultiPlayerWorld.h"
#include "Entities/Zombie.h"
#include "Util/EngineDefaults.h"
#include "Util/PerlinNoise.h"
#include <filesystem>
#include <glm/ext/scalar_constants.hpp>
#include <iostream>


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
        int x = coords.GetX() * Chunk::CHUNK_WIDTH;
        int y = coords.GetY() * Chunk::CHUNK_HEIGHT;
        int z = coords.GetZ() * Chunk::CHUNK_DEPTH;
        fileManager >> Chunks.emplace(std::piecewise_construct, std::forward_as_tuple(x, y, z), std::forward_as_tuple(x, y, z)).first->second;
    }
}

MultiPlayerWorld::MultiPlayerWorld(const uint16_t width, const uint16_t height, const uint16_t depth) : WorldTime(0), LevelWidth(width), LevelHeight(height), LevelDepth(depth)
{
    Entity::SetWorld(this);
    Chunk::SetWorld(this);
    for (uint16_t i = 0xFFFF; i != 0; i--)
    {
        EntityAvailableIDs.push(i);
    }
    EntityAvailableIDs.push(0);
    Init();
    for (int i = 0; i < 10; i++)
    {
        new Zombie(EngineDefaults::GetNext(width), static_cast<float>(LevelHeight + 3), EngineDefaults::GetNext(depth));
    }
    NetworkManager.Start();
}

uint16_t MultiPlayerWorld::RegisterEntity(Entity* entity)
{
    uint16_t index = EntityAvailableIDs.top();
    EntityAvailableIDs.pop();
    Entities.emplace(index, entity);
    EntitiesAdded.push_back(entity);
    return index;
}

void MultiPlayerWorld::RemoveEntity(const uint16_t id)
{
    EntitiesToRemove.push_back(id);
    const auto packetToSend = std::make_shared<Packet>(PacketHeader::ENTITY_LEAVE_WORLD_PACKET);
    *packetToSend << id;
    for (const std::shared_ptr<ConnectionToClientInterface>& connection : Connections | std::views::keys)
    {
        auto* const client = dynamic_cast<ConnectionToClient*>(connection.get());
        client->WritePacket(packetToSend);
    }
}

void MultiPlayerWorld::Tick()
{
    WorldTime++;
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
    for (const std::vector<std::shared_ptr<Packet>> packetsToSend = GetTickPackets(); const auto& val : packetsToSend)
    {
        for (const std::shared_ptr<ConnectionToClientInterface>& connection : Connections | std::views::keys)
        {
            auto* const client = dynamic_cast<ConnectionToClient*>(connection.get());
            client->WritePacket(val);
        }
    }
}

void MultiPlayerWorld::Run()
{
    //Send interesting packets
    for (Entity* entity : EntitiesAdded)
    {
        for (const std::shared_ptr<ConnectionToClientInterface>& connection : Connections | std::views::keys)
        {
            auto* const client = dynamic_cast<ConnectionToClient*>(connection.get());
            if (Connections[connection] != entity)
            {
                client->WritePacket(entity->GetSpawnPacket());
            }
        }
    }
    EntitiesAdded.clear();
    //Handle newly connected clients
    std::shared_ptr<ConnectionToClient> newCon = NetworkManager.GetNextNewConnection();
    while (newCon != nullptr)
    {
        const auto x = static_cast<float>(EngineDefaults::GetNext<int>(LevelWidth));
        const auto y = static_cast<float>(LevelHeight + 3);
        const auto z = static_cast<float>(EngineDefaults::GetNext<int>(LevelDepth));
        Connections.emplace(newCon, new Player{x, y, z, newCon});
        auto packet = std::make_shared<Packet>(PacketHeader::WORLD_TIME_PACKET);
        *packet << WorldTime;
        *packet << PartialTick;
        newCon->WritePacket(packet);
        SendEntireWorldToClient(newCon.get());
        newCon = NetworkManager.GetNextNewConnection();
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
    LightLevels.reserve(static_cast<size_t>(LevelWidth * LevelDepth));
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

void MultiPlayerWorld::SendEntireWorldToClient(ConnectionToClient* client) const
{
    const auto numChunks = static_cast<uint32_t>(Chunks.size());
    const auto numLights = static_cast<uint32_t>(LightLevels.size());
    const auto packet = std::make_shared<Packet>(PacketHeader(EPacketType::WorldData, numChunks * PacketHeader::CHUNK_DATA_PACKET.PacketSize +
                                                              numLights * PacketHeader::LIGHTS_DATA_PACKET.PacketSize + sizeof(uint32_t) * 2));
    *packet << numChunks << numLights;
    for (const auto& val : Chunks | std::views::values)
    {
        val.SendChunkToClient(packet);
    }
    for (const auto& [pos, level] : LightLevels)
    {
        *packet << pos.x << pos.y << level;
    }
    client->WritePacket(packet);
    for (const auto& val : Entities | std::views::values)
    {
        client->WritePacket(val->GetSpawnPacket());
    }
}

void MultiPlayerWorld::GenerateChunks(const uint16_t amountX, const uint16_t amountY, const uint16_t amountZ)
{
    Chunks.reserve(static_cast<uint64_t>(amountX) * static_cast<uint64_t>(amountY) * static_cast<uint64_t>(amountZ));
    for (int x = 0; x < amountX; x++)
    {
        for (int y = 0; y < amountY; y++)
        {
            for (int z = 0; z < amountZ; z++)
            {
                Chunks.emplace(std::piecewise_construct, std::forward_as_tuple(x * Chunk::CHUNK_WIDTH, y * Chunk::CHUNK_HEIGHT, z * Chunk::CHUNK_DEPTH), std::forward_as_tuple(x * Chunk::CHUNK_WIDTH, y * Chunk::CHUNK_HEIGHT, z * Chunk::CHUNK_DEPTH));
            }
        }
    }
}

void MultiPlayerWorld::GenerateLevel()
{
    const auto amountX = static_cast<uint16_t>(std::ceil(static_cast<float>(LevelWidth) / static_cast<float>(Chunk::CHUNK_WIDTH)));
    const auto amountY = static_cast<uint16_t>(std::ceil(static_cast<float>(LevelHeight) / static_cast<float>(Chunk::CHUNK_HEIGHT)));
    const auto amountZ = static_cast<uint16_t>(std::ceil(static_cast<float>(LevelDepth) / static_cast<float>(Chunk::CHUNK_DEPTH)));
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
    if (block->IsSolidBlock() != previousBlock->IsSolidBlock())
    {
        //UpdateChunksNear(x, y, z);
    }
    if (block->IsBlockingLight() != previousBlock->IsBlockingLight())
    {
        RecalculateLightLevels(x, z);
        //const int lightLevelsChange = RecalculateLightLevels(x, z);
        /*for (int i = 0; i <= abs(lightLevelsChange); i++)
        {
            if (Chunk* chunkLight = GetChunkAt(x, y + i * (lightLevelsChange > 0 ? -1 : 1), z); chunkLight != nullptr)
            {
                AddChunkAsDirty(chunkLight);
            }
        }*/
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
    if (block->IsSolidBlock())
    {
        //UpdateChunksNear(x, y, z);
    }
    if (block->IsBlockingLight())
    {
        RecalculateLightLevels(x, z);
        //const int lightLevelsChange = RecalculateLightLevels(x, z);
        /*for (int i = 0; i <= abs(lightLevelsChange); i++)
        {
            if (Chunk* chunkLight = GetChunkAt(x, y + i * (lightLevelsChange > 0 ? 1 : -1), z); chunkLight != nullptr)
            {
                AddChunkAsDirty(chunkLight);
            }
        }*/
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

std::vector<std::shared_ptr<Packet>> MultiPlayerWorld::GetTickPackets() const
{
    std::vector<std::shared_ptr<Packet>> result{};
    const auto packet = std::make_shared<Packet>(PacketHeader::WORLD_TIME_PACKET);
    *packet << WorldTime;
    *packet << PartialTick;
    result.push_back(packet);
    for (const std::unique_ptr<Entity>& entity : Entities | std::views::values)
    {
        result.push_back(entity->GetTickPacket());
    }
    return result;
}

void MultiPlayerWorld::HandlePacket(const PacketData* packet)
{
    if (const auto* keyboardPacket = dynamic_cast<const KeyChangePacket*>(packet); keyboardPacket != nullptr)
    {
        Connections[keyboardPacket->GetConnectionToClient()]->HandleKeyChangePacket(*keyboardPacket);
    }
    /*else if (const auto* mousePacket = dynamic_cast<const MouseChangePacket*>(packet); mousePacket != nullptr)
    {
        Connections[mousePacket->GetConnectionToClient()]->HandleMouseClickPacket(*mousePacket);
    }*/
    else if (const auto* mouseMovePacket = dynamic_cast<const MousePosChangePacket*>(packet); mouseMovePacket != nullptr)
    {
        Connections[mouseMovePacket->GetConnectionToClient()]->HandleMouseMovementPacket(*mouseMovePacket);
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
