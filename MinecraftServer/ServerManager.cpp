#include "ServerManager.h"
#include "Network/ConnectionToClient.h"
#include "Network/Packets/ClientInputPacket.h"
#include "Network/Packets/Packet.h"
#include <iostream>

void ServerManager::PreTick()
{
    World::PreTick();
    for (uint16_t id : DisconnectedClients)
    {
        dynamic_cast<Player*>(State.GetEntity<PlayerStateWrapper, PlayerState>(id))->Disconnect();
    }
    DisconnectedClients.clear();
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
}

void ServerManager::Run()
{
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
        Connections.emplace(newCon, dynamic_cast<Player*>(State.GetEntity<PlayerStateWrapper, PlayerState>(state.EntityId)));
        auto packet = std::make_shared<Packet>(PacketHeader::PLAYER_ID_PACKET);
        *packet << state.EntityId;
        for (const auto& key : Connections | std::ranges::views::keys)
        {
            dynamic_cast<ConnectionToClient*>(key.get())->WritePacket(packet);
        }
        std::vector<uint8_t> data{};
        State.WriteAllDataToVector(data);
        const auto packetToSend = std::make_shared<Packet>(PacketHeader(EPacketType::WorldData, static_cast<uint32_t>(data.size())), data);
        newCon->WritePacket(packetToSend);
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
        DisconnectedClients.emplace_back(Connections[closedCon]->GetState().EntityId);
        Connections.erase(closedCon);
        closedCon = NetworkManager.GetNextRemovedConnection();
    }
}

void ServerManager::NewTick()
{
    for (auto it = State.GetPlayersIterator(); it != State.GetState().Players.end(); ++it)
    {
        it->second->NewTick();
    }
    Tick();
}

void ServerManager::HandlePacket(const PacketData* packet)
{
    if (packet->GetPacketType() == EPacketType::ClientInput)
    {
        const uint64_t currentWorldTick = State.GetState().WorldTime;
        if (const auto* clientInputPacket = dynamic_cast<const ClientInputPacket*>(packet); RevertWorldState(clientInputPacket->GetWorldTickSent() - 1))
        {
            Connections[packet->GetConnectionToClient()]->SetClientInput(State.GetState().WorldTime + 1, clientInputPacket->GetState());
            Tick();
            std::vector<uint8_t> changes{};
            State.WriteChangesToVector(changes);
            const auto packetToSend = std::make_shared<Packet>(PacketHeader(EPacketType::WorldData, static_cast<uint32_t>(changes.size())), changes);
            for (const auto& key : Connections | std::ranges::views::keys)
            {
                dynamic_cast<ConnectionToClient*>(key.get())->WritePacket(packetToSend);
            }
            SimulateTicks(static_cast<uint8_t>(currentWorldTick - State.GetState().WorldTime));
        }
        else
        {
            std::cout << "Client is too far behind" << std::endl;
        }
    }
}
