#include "MultiPlayerWorld.h"
#include "Network/Packets/PlayerIdPacket.h"
#include "Network/Packets/WorldDataPacket.h"
#include <iostream>


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
    ApplyChangesList(dynamic_cast<WorldDataPacket*>(next.get())->GetData());
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
    ClientWorld::Run();
}

void MultiPlayerWorld::NewTick()
{
    const auto packet = std::make_shared<Packet>(PacketHeader::CLIENT_INPUT_HEADER);
    *packet << State.GetState().WorldTime + 1 << PlayerInput.Input;
    NetworkManager.WritePacket(packet);
    LastTickSentData = State.GetState().WorldTime + 1;
    ClientWorld::NewTick();
}

void MultiPlayerWorld::HandlePacket(const PacketData* packet)
{
    if (packet->GetPacketType() == EPacketType::PlayerId)
    {
        PlayerState state{};
        State.AddEntity(&state);
        EntityAdded(state.EntityId);
    }
    else
    {
        const auto* const worldDataPacket = dynamic_cast<const WorldDataPacket*>(packet);
        if (worldDataPacket->GetWorldTime() > LastTickSentData)
        {
            return;
        }
        const std::vector<uint8_t>& localChanges = ChangesLists[worldDataPacket->GetWorldTime() % ChangesLists.size()];
        const std::vector<uint8_t>& remoteChanges = worldDataPacket->GetData();
        if (remoteChanges.size() != localChanges.size() || std::memcmp(remoteChanges.data(), localChanges.data(), localChanges.size()) != 0)
        {
            /*
            for (size_t i = 0; i < worldDataPacket->GetData().size(); i++)
            {
                if (worldDataPacket->GetData()[i] != ChangesLists[(worldDataPacket->GetWorldTime()) % ChangesLists.size()][i])
                    std::cout << "Different at " << i << std::endl;
            }
            std::cout << "Different" << std::endl;*/
            State.ClearAllChanges();
            const size_t currentWorldTime = State.GetState().WorldTime;
            for (size_t i = currentWorldTime; i >= worldDataPacket->GetWorldTime(); i--)
            {
                RevertChangesList(ChangesLists[i % ChangesLists.size()]);
            }
            ApplyChangesList(remoteChanges);
            ChangesLists[State.GetState().WorldTime % ChangesLists.size()] = remoteChanges;
            assert(currentWorldTime - State.GetState().WorldTime < 25);
            SimulateTicks(static_cast<uint8_t>(currentWorldTime - State.GetState().WorldTime));
        }
    }
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
                    State.GetEntity<BlockParticleEntityStateWrapper, BlockParticleEntityState>(entityId)->ApplyRevertEntityChanges(changes, pos, true);
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
