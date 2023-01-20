#include "MultiPlayerWorld.h"
#include "Entities/Generic/Zombie.h"
#include "Entities/MP/OtherPlayer.h"
#include "Network/Packets/ChunkDataPacket.h"
#include "Network/Packets/EntityDataPacket.h"
#include "Network/Packets/EntityRemovedPacket.h"
#include "Network/Packets/EntitySpawnPacket.h"
#include "Network/Packets/LightDataPacket.h"
#include "Network/Packets/PlayerRotateChangePacket.h"
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
    const auto* spawnPacket = dynamic_cast<EntitySpawnPacket*>(packet.get());
    NextId = spawnPacket->GetEntityData().GetId();
    Player = new PlayerMp(spawnPacket->GetEntityData());
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
    std::shared_ptr<PacketData> packet = NetworkManager.GetNextPacket();
    while (packet != nullptr)
    {
        HandlePacket(packet.get());
        packet = NetworkManager.GetNextPacket();
    }
    DrawWorld(TickTimer / TICK_RATE);
    //handle messages
}

void MultiPlayerWorld::HandleKeyCallback(const int key, const int action)
{
    const auto packet = std::make_shared<Packet>(PacketHeader::KEYBOARD_PACKET);
    *packet << key << action;
    NetworkManager.WritePacket(packet);
}

void MultiPlayerWorld::HandleCursorPosCallback(const float xPos, const float yPos)
{
    Player->HandleMouseMovementInput(xPos, yPos, &NetworkManager);
}

void MultiPlayerWorld::HandleMouseButtonCallback(int button, int action)
{
}

void MultiPlayerWorld::HandlePacket(const PacketData* packet)
{
    switch (packet->GetPacketType())
    {
    case EPacketType::WorldTime:
        {
            const auto* worldTimePacket = dynamic_cast<const WorldTimePacket*>(packet);
            WorldTime = worldTimePacket->GetNewWorldTime();
            TickTimer = worldTimePacket->GetTicksTimer();
            break;
        }
    case EPacketType::EntityData:
        {
            if (const auto* entityData = dynamic_cast<const EntityDataPacket*>(packet); Entities.contains(entityData->GetId()))
            {
                Entities[entityData->GetId()]->HandleEntityUpdate(*entityData);
            }
            break;
        }
    case EPacketType::PlayerRotationChange:
        {
            const auto* playerRotChange = dynamic_cast<const PlayerRotateChangePacket*>(packet);
            Player->HandlePlayerRotationChange(*playerRotChange);
            break;
        }
    case EPacketType::WorldData:
        {
            const auto* worldData = dynamic_cast<const WorldDataPacket*>(packet);
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
            break;
        }
    case EPacketType::EntityEnterWorld:
        {
            const auto* newEntity = dynamic_cast<const EntitySpawnPacket*>(packet);
            NextId = newEntity->GetEntityData().GetId();
            switch (newEntity->GetEntityType())
            {
            case EEntityType::Zombie:
                new Zombie<MultiPlayerWorld>(newEntity->GetEntityData());
                break;
            case EEntityType::Player:
                {
                    const float x = newEntity->GetEntityData().GetXPos();
                    const float y = newEntity->GetEntityData().GetYPos();
                    const float z = newEntity->GetEntityData().GetZPos();
                    new OtherPlayer(x, y, z);
                    break;
                }
            case EEntityType::BlockBreakParticle:
                break;
            }
            break;
        }
    case EPacketType::EntityLeaveWorld:
        {
            const auto* entityLeavePacket = dynamic_cast<const EntityRemovedPacket*>(packet);
            Entities.erase(entityLeavePacket->GetId());
            break;
        }
    default:
        break;
    }
}

MultiPlayerWorld::~MultiPlayerWorld()
{
    glDeleteBuffers(1, &FogsBuffer);
}
