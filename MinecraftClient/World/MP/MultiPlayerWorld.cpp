#include "MultiPlayerWorld.h"
#include "Network/Packets/ChunkDataPacket.h"
#include "Network/Packets/EntityDataPacket.h"
#include "Network/Packets/LightDataPacket.h"
#include "Network/Packets/WorldDataPacket.h"
#include "Network/Packets/WorldTimePacket.h"


MultiPlayerWorld::MultiPlayerWorld(GLFWwindow* window, const std::string& name, const std::string& ip) : World(window, this), NextId(0)
{
    NetworkManager.Start(ip, name);
    Init();
}

uint16_t MultiPlayerWorld::RegisterEntity(Entity<MultiPlayerWorld>* entity)
{
    Entities.emplace(NextId, entity);
    return NextId;
}

void MultiPlayerWorld::Init()
{
    World::Init();
    std::shared_ptr<PacketData> packet = NetworkManager.GetNextPacket();
    while (packet == nullptr)
    {
        packet = NetworkManager.GetNextPacket();
    }
    const auto* positionPacket = dynamic_cast<EntityDataPacket*>(packet.get());
    Player = new PlayerMp(*positionPacket);
    Player->GetTransform().SetRotation(0.0F, positionPacket->GetYRot(), 0.0F);
    //Do more stuff here
}

void MultiPlayerWorld::UpdateChunksNear(const int x, const int y, const int z)
{
    Chunk<MultiPlayerWorld>* chunk = GetChunkAt(x, y + 1, z);
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

void MultiPlayerWorld::Run()
{
    if (TickTimer > 1.0F)
    {
        TickTimer = 1.0F;
    }
    DrawWorld(TickTimer);
    //handle messages
    std::shared_ptr<PacketData> packet = NetworkManager.GetNextPacket();
    while (packet != nullptr)
    {
        HandlePacket(packet.get());
        packet = NetworkManager.GetNextPacket();
    }
}

void MultiPlayerWorld::HandlePacket(const PacketData* packet)
{
    if (const auto* worldTimePacket = dynamic_cast<const WorldTimePacket*>(packet); worldTimePacket != nullptr)
    {
        WorldTime = worldTimePacket->GetNewWorldTime();
        TickTimer = 0.0F;
    }
    else if (const auto* entityData = dynamic_cast<const EntityDataPacket*>(packet); entityData != nullptr)
    {
        Entities[entityData->GetId()]->HandleEntityUpdate(*entityData);
    }
    else if (const auto* worldData = dynamic_cast<const WorldDataPacket*>(packet); worldData != nullptr)
    {
        for (const auto& lights = worldData->GetLights(); const auto& light : lights)
        {
            LightLevels.emplace(glm::ivec2(light->GetX(), light->GetZ()), light->GetLight());
        }
        for (const auto& chunks = worldData->GetChunks(); const auto& chunk : chunks)
        {
            int x = chunk->GetX() * Chunk<MultiPlayerWorld>::CHUNK_WIDTH;
            int y = chunk->GetY() * Chunk<MultiPlayerWorld>::CHUNK_HEIGHT;
            int z = chunk->GetZ() * Chunk<MultiPlayerWorld>::CHUNK_DEPTH;
            Chunk<MultiPlayerWorld>& chunkCreated = Chunks.emplace(std::piecewise_construct, std::forward_as_tuple(x, y, z), std::forward_as_tuple(x, y, z)).first->second;
            size_t counter = 0;
            auto blocks = chunk->GetChunkData();
            for (y = 0; y < Chunk<MultiPlayerWorld>::CHUNK_HEIGHT; y++)
            {
                for (z = 0; z < Chunk<MultiPlayerWorld>::CHUNK_DEPTH; z++)
                {
                    for (x = 0; x < Chunk<MultiPlayerWorld>::CHUNK_WIDTH; x++)
                    {
                        chunkCreated.SetBlockTypeAt(x, y, z, static_cast<EBlockType>(blocks[counter]));
                        counter++;
                    }
                }
            }
        }
    }
}

MultiPlayerWorld::~MultiPlayerWorld()
{
    glDeleteBuffers(1, &FogsBuffer);
}
