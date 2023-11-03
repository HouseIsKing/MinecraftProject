#include "ServerManager.h"
#include "Network/ConnectionToClient.h"
#include "Network/Packets/ClientInputPacket.h"
#include "Network/Packets/Packet.h"
#include <iostream>
#include <GLFW/glfw3.h>

void ServerManager::PreTick()
{
    World::PreTick();
    //Handle newly connected clients
    std::shared_ptr<ConnectionToClient> newCon = NetworkManager.GetNextNewConnection();
    while (newCon)
    {
        //CustomRandomEngine random;
        //const auto x = static_cast<float>(random.GetNext(LevelWidth));
        //const auto y = static_cast<float>(LevelHeight + 3);
        //const auto z = static_cast<float>(random.GetNext(LevelDepth));
        PlayerState state{};
        state.EntityTransform.Position = glm::vec3(0.0F, 70.0F, 0.0F);
        State.AddEntity(&state);
        auto packetToSend = std::make_shared<Packet>(PacketHeader::PLAYER_ID_PACKET);
        *packetToSend << state.EntityId;
        newCon->WritePacket(packetToSend, newCon);
        Connections.emplace(newCon, dynamic_cast<Player*>(State.GetEntity<PlayerStateWrapper, PlayerState>(state.EntityId)));
        std::vector<uint8_t> data{};
        uint64_t temp = 0;
        EngineDefaults::EmplaceReplaceDataInVector(data, &temp);
        State.WriteAllDataToVector(data);
        temp = data.size() - sizeof(temp);
        EngineDefaults::EmplaceReplaceDataInVector(data, &temp, 0);
        std::vector<uint8_t> compressedData = EngineDefaults::Deflate(data);
        std::vector<uint8_t> dataToSend{};
        EngineDefaults::EmplaceReplaceDataInVector(dataToSend, &temp);
        dataToSend.emplace_back(static_cast<uint8_t>(1));
        dataToSend.insert(dataToSend.end(), compressedData.begin(), compressedData.end());
        packetToSend = std::make_shared<Packet>(PacketHeader(EPacketType::WorldData, dataToSend.size()), dataToSend);
        newCon->WritePacket(packetToSend, newCon);
        newCon = NetworkManager.GetNextNewConnection();
    }
    //Handle packets
    std::shared_ptr<PacketData> nextPacket = NetworkManager.GetNextPacket();
    while (nextPacket)
    {
        HandlePacket(nextPacket.get());
        nextPacket = NetworkManager.GetNextPacket();
    }
    //Handle disconnected clients
    std::shared_ptr<ConnectionToClient> closedCon = NetworkManager.GetNextRemovedConnection();
    while (closedCon)
    {
        //Plan remove entity
        //EntitiesToRemove.emplace_back(Connections[closedCon]->GetState().EntityId);
        State.RemoveEntity(Connections[closedCon]->GetState().EntityId);
        Connections.erase(closedCon);
        closedCon = NetworkManager.GetNextRemovedConnection();
    }
}

ServerManager::~ServerManager()
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

ServerManager::ServerManager(const uint16_t width, const uint16_t height, const uint16_t depth) : World(width, height, depth)
{
    LoadWorld();
    NetworkManager.Start();
    StartTime = static_cast<float>(glfwGetTime());
}

void ServerManager::Run()
{
    int i;
    for (i = 0; i < static_cast<int>(TickTimer / EngineDefaults::TICK_RATE); i++)
    {
        NewTick();
    }
    const auto end = static_cast<float>(glfwGetTime());
    TickTimer += end - StartTime - static_cast<float>(i) * EngineDefaults::TICK_RATE;
    StartTime = end;
}

void ServerManager::NewTick()
{
    Tick();
    for (auto [con, player] : Connections)
    {
        if (con->LastInputProcessed != player->GetLastInputProcessed())
        {
            std::vector<uint8_t> dataToSend{};
            uint64_t inputId = player->GetLastInputProcessed();
            EngineDefaults::EmplaceReplaceDataInVector(dataToSend, &inputId);
            auto ticksCount = static_cast<uint8_t>(GetWorldTime() - con->LastTickSent);
            EngineDefaults::EmplaceReplaceDataInVector(dataToSend, &ticksCount);
            std::vector<uint8_t> dataUncompressed;
            for (uint64_t tick = con->LastTickSent + 1; tick <= GetWorldTime(); tick++)
            {
                uint64_t size = ChangesLists[tick % EngineDefaults::ROLLBACK_COUNT].size();
                EngineDefaults::EmplaceReplaceDataInVector(dataUncompressed, &size);
                dataUncompressed.insert(dataUncompressed.end(), ChangesLists[tick % EngineDefaults::ROLLBACK_COUNT].begin(), ChangesLists[tick % EngineDefaults::ROLLBACK_COUNT].end());
            }
            std::vector<uint8_t> compressedData = EngineDefaults::Deflate(dataUncompressed);
            dataToSend.insert(dataToSend.end(), compressedData.begin(), compressedData.end());
            const auto packetToSend = std::make_shared<Packet>(PacketHeader(EPacketType::WorldData, static_cast<uint32_t>(dataToSend.size())), dataToSend);
            reinterpret_cast<const ConnectionToClient*>(con.get())->WritePacket(packetToSend, std::reinterpret_pointer_cast<ConnectionToClient, ConnectionToClientInterface>(con));
            con->LastTickSent = GetWorldTime();
            con->LastInputProcessed = player->GetLastInputProcessed();
        }
    }
}

void ServerManager::HandlePacket(const PacketData* packet)
{
    if (packet->GetPacketType() == EPacketType::ClientInput)
    {
        const auto* const inputPacket = dynamic_cast<const ClientInputPacket*>(packet);
        Connections[packet->GetConnectionToClient()]->AddClientInputToQueue(inputPacket->GetState(), inputPacket->GetInputId());
    }
}
