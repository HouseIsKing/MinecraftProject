#include "MultiPlayerWorld.h"

#include <iostream>

#include "Entities/Generic/Zombie.h"
#include "Entities/MP/OtherPlayer.h"
#include "Entities/MP/PlayerMp.h"
#include "Network/Packets/PlayerIdPacket.h"
#include "Network/Packets/WorldDataPacket.h"


MultiPlayerWorld::MultiPlayerWorld(GLFWwindow* window, const std::string& name, const std::string& ip) : World(window, 64, 256, 256), NextId(0)
{
    NetworkManager.Start(ip, name);
    Init();
}

void MultiPlayerWorld::Init()
{
    std::shared_ptr<PacketData> packet = NetworkManager.GetNextPacket();
    while (packet == nullptr)
    {
        packet = NetworkManager.GetNextPacket();
    }
    World::Init();
    PlayerId = reinterpret_cast<PlayerIdPacket*>(packet.get())->GetPlayerId();
    const auto playerInputPacket = std::make_shared<Packet>(PacketHeader::CLIENT_INPUT_HEADER);
    *playerInputPacket << 0ULL;
    *playerInputPacket << ClientInputState();
    NetworkManager.WritePacket(playerInputPacket);
    packet = NetworkManager.GetNextPacket();
    while (packet == nullptr)
    {
        packet = NetworkManager.GetNextPacket();
    }
    std::cout << "Received world data" << std::chrono::system_clock::now() << std::endl;
    const auto* worldDataPacket = dynamic_cast<WorldDataPacket*>(packet.get());
    HandlePacket(worldDataPacket);
    BuildWorldState();
}

void MultiPlayerWorld::Run()
{
    std::shared_ptr<PacketData> packet = NetworkManager.GetNextPacket();
    while (packet != nullptr)
    {
        HandlePacket(packet.get());
        packet = NetworkManager.GetNextPacket();
    }
    uint8_t i;
    for (i = 0; i < static_cast<uint8_t>(TickTimer / EngineDefaults::TICK_RATE); i++)
    {
        auto* player = reinterpret_cast<PlayerMp*>(Entities.at(PlayerId).get());
        player->CopyInputStateToHistory(WorldTime + 1);
        const ClientInputState& state = player->GetInputState(WorldTime + 1);
        auto playerInputPacket = std::make_shared<Packet>(PacketHeader::CLIENT_INPUT_HEADER);
        *playerInputPacket << WorldTime + 1;
        *playerInputPacket << state;
        NetworkManager.WritePacket(playerInputPacket);
        std::cout << "Sent input packet" << std::chrono::system_clock::now() << std::endl;
        Tick();
    }
    TickTimer -= static_cast<float>(i) * EngineDefaults::TICK_RATE;
    DrawWorld(TickTimer / EngineDefaults::TICK_RATE);
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

void MultiPlayerWorld::BuildWorldState()
{
    WorldState& worldState = WorldStates[WorldTime % WorldStates.size()];
    worldState.Chunks.clear();
    for (Chunk& chunk : Chunks | std::views::values)
    {
        worldState.Chunks.emplace_hint(worldState.Chunks.end(), chunk.GetChunkPos(), chunk.GetChunkState());
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

void MultiPlayerWorld::Tick()
{
    World::Tick();
    BuildWorldState();
}


void MultiPlayerWorld::SimulateTicks(const uint8_t tickCount)
{
    for (uint8_t i = 0; i < tickCount; i++)
    {
        Tick();
    }
}


void MultiPlayerWorld::HandlePacket(const PacketData* packet)
{
    switch (packet->GetPacketType())
    {
    case EPacketType::WorldData:
        {
            const auto* worldDataPacket = reinterpret_cast<const WorldDataPacket*>(packet);
            if (const uint64_t currentWorldTick = WorldTime; currentWorldTick == 0)
            {
                ApplyChangesList(worldDataPacket->GetData());
            }
            else if (worldDataPacket->GetWorldTime() >= WorldTime + 1 - WorldStates.size())
            {
                std::cout << "Handling packet" << std::endl;
                const std::vector<uint8_t> changesPredicted = WorldStates[(worldDataPacket->GetWorldTime() - 1) % WorldStates.size()] - WorldStates[worldDataPacket->GetWorldTime() % WorldStates.size()];
                if (changesPredicted.size() == worldDataPacket->GetData().size() && memcmp(worldDataPacket->GetData().data(), changesPredicted.data(), changesPredicted.size()) == 0)
                {
                    break;
                }
                RevertWorldState(worldDataPacket->GetWorldTime() - 1);
                ApplyChangesList(worldDataPacket->GetData());
                BuildWorldState();
                SimulateTicks(static_cast<uint8_t>(currentWorldTick - WorldTime));
            }
            else
            {
                std::cout << "Server way too far behind" << std::endl;
            }
            break;
        }
    default:
        break;
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
                        if (playerState->EntityId == PlayerId)
                        {
                            Entities.emplace(playerState->EntityId, new PlayerMp(*playerState));
                        }
                        else
                        {
                            Entities.emplace(playerState->EntityId, new OtherPlayer(*playerState));
                        }
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

MultiPlayerWorld::~MultiPlayerWorld()
{
    glDeleteBuffers(1, &FogsBuffer);
}
