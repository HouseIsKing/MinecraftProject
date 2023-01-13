#include "ClientNetworkManager.h"
#include "Packets/ConnectionInitPacket.h"

ServerNetworkManager::ServerNetworkManager() : Socket(Context)
{
}

ServerNetworkManager::~ServerNetworkManager()
{
    Context.stop();
    ContextThread.join();
}

void ServerNetworkManager::Start(const std::string ip, std::string name)
{
    asio::error_code ec;
    Socket.connect(asio::ip::tcp::endpoint(asio::ip::address::from_string(ip), 25565), ec);
    ConnectionInitPacket packetToSend{std::move(name)};
    Packet packet = packetToSend.GetPacketAsSend();
    Socket.write_some(asio::buffer(packet.GetHeader().Serialize(), sizeof(PacketHeader)), ec);
    Socket.write_some(asio::buffer(packet.GetData(), packet.GetHeader().PacketSize), ec);
}

std::shared_ptr<PacketData> ServerNetworkManager::GetNextPacket()
{
    if (Packets.GetSize() > 0)
    {
        return Packets.Pop();
    }
    return nullptr;
}
