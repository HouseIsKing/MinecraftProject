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
    ClientWorld::NewTick();
}

void MultiPlayerWorld::HandlePacket(const PacketData* packet)
{
    const auto* const worldDataPacket = dynamic_cast<const WorldDataPacket*>(packet);
    const std::vector<uint8_t>& localChanges = ChangesLists[worldDataPacket->GetWorldTime() % ChangesLists.size()];
    const std::vector<uint8_t>& remoteChanges = worldDataPacket->GetData();
    if (remoteChanges.size() != localChanges.size() || std::memcmp(remoteChanges.data(), localChanges.data(), localChanges.size()) != 0)
    {
        for (size_t i = 0; i < worldDataPacket->GetData().size(); i++)
        {
            if (worldDataPacket->GetData()[i] != ChangesLists[(worldDataPacket->GetWorldTime()) % ChangesLists.size()][i])
                std::cout << "Different at " << i << std::endl;
        }
        std::cout << "Different" << std::endl;
    }
}

void MultiPlayerWorld::ApplyChangesList(const std::vector<uint8_t>& changes)
{
    size_t pos = 0;
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
        default:
            break;
        }
    }
    State.ClearAllChanges();
}

MultiPlayerWorld::~MultiPlayerWorld()
{
    glDeleteBuffers(1, &FogsBuffer);
}
