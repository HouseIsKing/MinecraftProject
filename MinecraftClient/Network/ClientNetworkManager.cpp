#include "ClientNetworkManager.h"

#include <iostream>
#include <utility>

#include "Packets/ConnectionInitPacket.h"

ClientNetworkManager::ClientNetworkManager() : Socket(Context)
{
}

ClientNetworkManager::~ClientNetworkManager()
{
    Context.stop();
    ContextThread.join();
}

void ClientNetworkManager::Start(const std::string ip, std::string name)
{
    asio::error_code ec;
    Socket.connect(asio::ip::tcp::endpoint(asio::ip::address::from_string(ip), 25565), ec);
    ConnectionInitPacket packetToSend{std::move(name)};
    Packet packet = packetToSend.GetPacketAsSend();
    Socket.write_some(asio::buffer(packet.GetHeader().Serialize(), sizeof(PacketHeader)), ec);
    Socket.write_some(asio::buffer(packet.GetData(), packet.GetHeader().PacketSize), ec);
}

std::shared_ptr<PacketData> ClientNetworkManager::GetNextPacket()
{
    if (Packets.GetSize() > 0)
    {
        return Packets.Pop();
    }
    return nullptr;
}
