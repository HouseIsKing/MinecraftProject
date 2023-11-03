#include "MultiPlayerWorld.h"
#include "Network/Packets/PlayerIdPacket.h"
#include "Network/Packets/WorldDataPacket.h"
#include <iostream>

#include "Network/Packets/OtherPlayerInputPacket.h"


MultiPlayerWorld::MultiPlayerWorld(GLFWwindow* window, const std::string& name, const std::string& ip) : ClientWorld(window, 64, 256, 256)
{
    NetworkManager.Start(ip, name);
    std::shared_ptr<PacketData> next = NetworkManager.GetNextPacket();
    while (!next)
    {
        next = NetworkManager.GetNextPacket();
    }
    PlayerId = dynamic_cast<PlayerIdPacket*>(next.get())->GetPlayerId();
    next = NetworkManager.GetNextPacket();
    while (!next)
    {
        next = NetworkManager.GetNextPacket();
    }
    ApplyChangesList(dynamic_cast<WorldDataPacket*>(next.get())->GetData()[0]);
    LastTimeFrame = static_cast<float>(glfwGetTime());
}

void MultiPlayerWorld::Run()
{
    std::shared_ptr<PacketData> next = NetworkManager.GetNextPacket();
    while (next)
    {
        HandlePacket(next.get());
        next = NetworkManager.GetNextPacket();
    }
    if (TickTimer >= EngineDefaults::TICK_RATE)
    {
        PacketId++;
        const auto packet = std::make_shared<Packet>(PacketHeader::CLIENT_INPUT_HEADER);
        *packet << PacketId;
        *packet << PlayerInput.Input;
        NetworkManager.WritePacket(packet);
        InputsSentButNotConfirmed.emplace(PacketId, PlayerInput.Input);
    }
    ClientWorld::Run();
}

void MultiPlayerWorld::HandlePacket(const PacketData* packet)
{
    const auto* worldDataPacket = dynamic_cast<const WorldDataPacket*>(packet);
    /*
    if (SkippedTicksCount == EngineDefaults::ROLLBACK_COUNT)
    {
        for (size_t i = 0; i < EngineDefaults::ROLLBACK_COUNT; ++i)
        {
            worldDataPacket = dynamic_cast<const WorldDataPacket*>(PacketsRead.front().get());
            const std::vector<uint8_t>& remoteChanges = worldDataPacket->GetData();
            State.ClearAllChanges();
            ApplyChangesList(remoteChanges);
            ChangesLists[State.GetState().WorldTime % ChangesLists.size()] = remoteChanges;
            PacketsRead.pop();
        }
        SkippedTicksCount = 0;
        TickTimer = 0;
        LastTimeFrame = static_cast<float>(glfwGetTime());
    }
    */
    //std::cout << "Server Processed Tick " << worldDataPacket->GetLastTickServerProcessed() << std::endl;
   /* if (worldDataPacket->GetLastTickServerProcessed() + EngineDefaults::ROLLBACK_COUNT > PacketId)
    {
        InputsSentButNotConfirmed.erase(InputsSentButNotConfirmed.begin(), InputsSentButNotConfirmed.upper_bound(worldDataPacket->GetLastTickServerProcessed()));
        const std::vector<uint8_t>& remoteChanges = worldDataPacket->GetData();
        RevertWorldState(State.GetState().WorldTime - InputsSentButNotConfirmed.size() - 1);
        State.ClearAllChanges();
        ApplyChangesList(remoteChanges);
        ChangesLists[worldDataPacket->GetLastTickServerProcessed() % ChangesLists.size()] = remoteChanges;
        SimulateTicks(InputsSentButNotConfirmed, PlayerId);
    }
    else
    {
        InputsSentButNotConfirmed.erase(InputsSentButNotConfirmed.begin(), InputsSentButNotConfirmed.upper_bound(worldDataPacket->GetLastTickServerProcessed()));
        ApplyChangesList(worldDataPacket->GetData());
        ChangesLists[worldDataPacket->GetLastTickServerProcessed() % ChangesLists.size()] = worldDataPacket->GetData();
    }*/
    //State.ClearAllChanges();
    if (worldDataPacket->GetLastInputIdServerProcessed() != 0)
    {
        InputsSentButNotConfirmed.erase(worldDataPacket->GetLastInputIdServerProcessed());
        RevertWorldState(State.GetState().WorldTime - InputsSentButNotConfirmed.size() - 1);
        State.ClearAllChanges();
        for (const std::vector<uint8_t>& data : worldDataPacket->GetData())
        {
            ApplyChangesList(data);
            ChangesLists[State.GetState().WorldTime % ChangesLists.size()] = data;
        }
        SimulateTicks(InputsSentButNotConfirmed, PlayerId);
    }
    /*
    else
    {
        SkippedTicksCount++;
    } */
}

void MultiPlayerWorld::ApplyChangesList(const std::vector<uint8_t>& changes)
{
    size_t pos = sizeof uint16_t;
    std::unordered_set<uint16_t> removedEntities{};
    while (pos < changes.size())
    {
        switch (EngineDefaults::ReadDataFromVector<EChangeType>(changes, pos))
        {
        case EChangeType::ChunkEnterWorld:
            {
                const uint32_t count = EngineDefaults::ReadDataFromVector<uint32_t>(changes, pos);
                for (uint32_t i = 0; i < count; ++i)
                {
                    ChunkState state{};
                    state.Deserialize(changes, pos);
                    State.AddChunk(state);
                    ChunkAdded(state.ChunkPosition);
                }
                break;
            }
        case EChangeType::ChunkLeaveWorld:
            {
                const uint32_t count = EngineDefaults::ReadDataFromVector<uint32_t>(changes, pos);
                for (uint32_t i = 0; i < count; ++i)
                {
                    ChunkState state{};
                    state.Deserialize(changes, pos);
                    State.RemoveChunk(state.ChunkPosition);
                    ChunkRemoved(state.ChunkPosition);
                }
                break;
            }
        case EChangeType::LightEnterWorld:
            {
                const uint32_t count = EngineDefaults::ReadDataFromVector<uint32_t>(changes, pos);
                for (uint32_t i = 0; i < count; ++i)
                {
                    auto lightPos = EngineDefaults::ReadDataFromVector<glm::ivec2>(changes, pos);
                    const auto light = EngineDefaults::ReadDataFromVector<uint8_t>(changes, pos);
                    State.AddLight(lightPos, light);
                }
                break;
            }
        case EChangeType::PlayerEnterWorld:
            {
                const uint32_t count = EngineDefaults::ReadDataFromVector<uint32_t>(changes, pos);
                for (uint32_t i = 0; i < count; ++i)
                {
                    PlayerState state{};
                    state.Deserialize(changes, pos);
                    State.AddEntity(&state, true);
                    EntityAdded(state.EntityId);
                }
                break;
            }
        case EChangeType::PlayerLeaveWorld:
            {
                const uint32_t count = EngineDefaults::ReadDataFromVector<uint32_t>(changes, pos);
                for (uint32_t i = 0; i < count; ++i)
                {
                    PlayerState state{};
                    state.Deserialize(changes, pos);
                    State.RemoveEntity(state.EntityId);
                    EntityRemoved(state.EntityId);
                }
                break;
            }
        case EChangeType::WorldTime:
            {
                pos += sizeof uint64_t;
                State.SetWorldTime(EngineDefaults::ReadDataFromVector<uint64_t>(changes, pos));
                break;
            }
        case EChangeType::RandomState:
            {
                pos += sizeof uint64_t;
                State.SetRandomSeed(EngineDefaults::ReadDataFromVector<uint64_t>(changes, pos));
                break;
            }
        case EChangeType::ChunkState:
            {
                State.GetChunk(EngineDefaults::ReadDataFromVector<ChunkCoords>(changes, pos))->ApplyRevertChunkChanges(changes, pos, false);
                break;
            }
        case EChangeType::BlockParticleEnterWorld:
            {
                const uint32_t changeCount = EngineDefaults::ReadDataFromVector<uint32_t>(changes, pos);
                for (uint32_t i = 0; i < changeCount; i++)
                {
                    BlockParticleEntityState state{};
                    state.Deserialize(changes, pos);
                    State.AddEntity(&state, true);
                    EntityAdded(state.EntityId);
                }
                break;
            }
        case EChangeType::BlockParticleLeaveWorld:
            {
                const uint32_t changeCount = EngineDefaults::ReadDataFromVector<uint32_t>(changes, pos);
                for (uint32_t i = 0; i < changeCount; i++)
                {
                    BlockParticleEntityState state{};
                    state.Deserialize(changes, pos);
                    removedEntities.emplace(state.EntityId);
                    State.RemoveEntity(state.EntityId);
                    EntityRemoved(state.EntityId);
                }
                break;
            }
        case EChangeType::LightLeaveWorld:
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
        case EChangeType::LightState:
            {
                const uint32_t count = EngineDefaults::ReadDataFromVector<uint32_t>(changes, pos);
                for (uint32_t i = 0; i < count; i++)
                {
                    const glm::ivec2 lightPos = EngineDefaults::ReadDataFromVector<glm::ivec2>(changes, pos);
                    pos += sizeof(uint8_t);
                    const uint8_t lightLevel = EngineDefaults::ReadDataFromVector<uint8_t>(changes, pos);
                    const uint8_t previousLightLevel = State.GetState().Lights.at(lightPos);
                    LightChanged(lightPos.x, previousLightLevel, lightPos.y, static_cast<int16_t>(lightLevel - previousLightLevel));
                    State.ChangeLight(lightPos, lightLevel);
                }
                break;
            }
        case EChangeType::PlayerState:
            {
                if (const auto& player = EngineDefaults::ReadDataFromVector<uint16_t>(changes, pos); !removedEntities.contains(player))
                {
                    State.GetEntity<PlayerStateWrapper, PlayerState>(player)->ApplyRevertEntityChanges(changes, pos, false);
                }
                else
                {
                    PlayerTrash.ApplyRevertEntityChanges(changes, pos, false);
                }
                break;
            }
        case EChangeType::BlockParticleState:
            {
                if (uint16_t entityId = EngineDefaults::ReadDataFromVector<uint16_t>(changes, pos); !removedEntities.contains(entityId))
                {
                    State.GetEntity<BlockParticleEntityStateWrapper, BlockParticleEntityState>(entityId)->ApplyRevertEntityChanges(changes, pos, false);
                }
                else
                {
                    BlockParticleTrash.ApplyRevertEntityChanges(changes, pos, true);
                }
                break;
            }
        }
    }
}

MultiPlayerWorld::~MultiPlayerWorld()
{
    glDeleteBuffers(1, &FogsBuffer);
}
