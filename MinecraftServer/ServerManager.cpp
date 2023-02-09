#include "ServerManager.h"
#include "Network/ConnectionToClient.h"
#include "Network/Packets/ClientInputPacket.h"
#include "Network/Packets/Packet.h"
#include <iostream>

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
    while (newCon != nullptr)
    {
        //CustomRandomEngine random;
        //const auto x = static_cast<float>(random.GetNext(LevelWidth));
        //const auto y = static_cast<float>(LevelHeight + 3);
        //const auto z = static_cast<float>(random.GetNext(LevelDepth));
        const uint16_t id = EntityAvailableIDs.top();
        EntityAvailableIDs.pop();
        State.AddPlayer(id, 0.0F, 70.0F, 0.0F);
        Connections.emplace(newCon, &State.GetPlayer(id));
        auto packet = std::make_shared<Packet>(PacketHeader::PLAYER_ID_PACKET);
        *packet << id;
        newCon->WritePacket(packet);
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
        State.RemovePlayer(Connections[closedCon]->GetEntityId());
        Connections.erase(closedCon);
        closedCon = NetworkManager.GetNextRemovedConnection();
    }
}

void ServerManager::NewTick()
{
    for (auto it = State.GetPlayersIterator(); it != State.GetState().Players.end(); ++it)
    {
        it->second.NewTick();
    }
    Tick();
}

void ServerManager::HandlePacket(const PacketData* packet)
{
    if (packet->GetPacketType() == EPacketType::ClientInput)
    {
        const uint64_t currentWorldTick = State.GetState().WorldTime;
        if (const auto* clientInputPacket = reinterpret_cast<const ClientInputPacket*>(packet); clientInputPacket->GetWorldTickSent() == 0)
        {
            std::vector<uint8_t> data{};
            State.WriteAllDataToVector(data);
            const auto packetToSend = std::make_shared<Packet>(PacketHeader(EPacketType::WorldData, static_cast<uint32_t>(data.size())), data);
            packet->GetConnectionToClient()->WritePacket(packetToSend);
        }
        else if (RevertWorldState(clientInputPacket->GetWorldTickSent() - 1))
        {
            Connections[packet->GetConnectionToClient()]->SetClientInput(State.GetState().WorldTime, clientInputPacket->GetState());
            Tick();
            std::vector<uint8_t> changes{};
            State.WriteChangesToVector(changes);
            const auto packetToSend = std::make_shared<Packet>(PacketHeader(EPacketType::WorldData, static_cast<uint32_t>(changes.size())), changes);
            packet->GetConnectionToClient()->WritePacket(packetToSend);
            SimulateTicks(static_cast<uint8_t>(currentWorldTick - State.GetState().WorldTime));
        }
        else
        {
            std::cout << "Client is too far behind" << std::endl;
        }
    }
}
