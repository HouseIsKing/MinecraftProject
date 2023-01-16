#include "ClientNetworkManager.h"

#include <iostream>

#include "Packets/EntityPositionPacket.h"
#include "Packets/PlayerIdPacket.h"
#include "Packets/WorldTimePacket.h"

void ClientNetworkManager::ReadPacketBodyAsync()
{
    Socket.async_read_some(asio::buffer(CurrentPacket.GetData(), CurrentPacket.GetHeader().PacketSize), [this](const asio::error_code& error, std::size_t
                           /*length*/)
                           {
                               if (!error)
                               {
                                   Packets.Push(TranslatePacket());
                                   ReadPacketHeaderAsync();
                               }
                               else
                               {
                                   std::cout << "Error reading packet body: " << error.message() << std::endl;
                               }
                           });
}

void ClientNetworkManager::ReadPacketHeaderAsync()
{
    Socket.async_read_some(asio::buffer(HeaderBuffer, HeaderBuffer.size()), [this](const std::error_code ec, std::size_t /*length*/)
    {
        if (!ec)
        {
            const PacketHeader header = *reinterpret_cast<PacketHeader*>(HeaderBuffer.data());
            CurrentPacket = Packet{header};
            ReadPacketBodyAsync();
        }
        else
        {
            std::cout << "Error reading packet header: " << ec.message() << std::endl;
        }
    });
}

std::shared_ptr<PacketData> ClientNetworkManager::TranslatePacket()
{
    switch (CurrentPacket.GetHeader().PacketType)
    {
    case EPacketType::WorldTime:
        return std::make_shared<WorldTimePacket>(CurrentPacket);
    case EPacketType::PlayerId:
        return std::make_shared<PlayerIdPacket>(CurrentPacket);
    case EPacketType::EntityPosition:
        return std::make_shared<EntityPositionPacket>(CurrentPacket);
    default:
        return nullptr;
    }
}

ClientNetworkManager::ClientNetworkManager() : Socket(Context), CurrentPacket(PacketHeader::PLAYER_ID_PACKET)
{
    HeaderBuffer.resize(sizeof(PacketHeader));
}

ClientNetworkManager::~ClientNetworkManager()
{
    Context.stop();
    ContextThread.join();
}

void ClientNetworkManager::Start(const std::string& ip, const std::string& name)
{
    asio::error_code ec;
    Socket.connect(asio::ip::tcp::endpoint(asio::ip::address::from_string(ip), 25565), ec);
    CurrentPacket = Packet{PacketHeader(EPacketType::PlayerId, static_cast<uint16_t>(name.size()))};
    CurrentPacket << name;
    Socket.write_some(asio::buffer(CurrentPacket.GetHeader().Serialize(), sizeof(PacketHeader)), ec);
    Socket.write_some(asio::buffer(CurrentPacket.GetData(), CurrentPacket.GetHeader().PacketSize), ec);
}

std::shared_ptr<PacketData> ClientNetworkManager::GetNextPacket()
{
    if (Packets.GetSize() > 0)
    {
        return Packets.Pop();
    }
    return nullptr;
}

void ClientNetworkManager::WritePacket(Packet& packet)
{
    Socket.async_write_some(asio::buffer(packet.GetHeader().Serialize(), sizeof(PacketHeader)), [](const asio::error_code& error, std::size_t /*bytesTransferred*/)
    {
        if (error)
        {
            std::cout << "Error writing packet header to server " << std::endl;
        }
    });
    Socket.async_write_some(asio::buffer(packet.GetData(), packet.GetHeader().PacketSize), [](const asio::error_code& error, std::size_t /*bytesTransferred*/)
    {
        if (error)
        {
            std::cout << "Error writing packet header to server " << std::endl;
        }
    });
}
