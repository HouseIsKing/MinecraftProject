#include "ClientNetworkManager.h"

#include <iostream>

#include "Packets/ChunkDataPacket.h"
#include "Packets/EntityDataPacket.h"
#include "Packets/LightDataPacket.h"
#include "Packets/PlayerRotateChangePacket.h"
#include "Packets/WorldDataPacket.h"
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

void ClientNetworkManager::WritePacketHeaderAsync()
{
    Socket.async_write_some(asio::buffer(OutgoingPackets.Front()->GetHeader().Serialize(), sizeof(PacketHeader)), [this](const asio::error_code& error, std::size_t /*bytesTransferred*/)
    {
        if (!error)
        {
            WritePacketBodyAsync();
        }
    });
}

void ClientNetworkManager::WritePacketBodyAsync()
{
    Socket.async_write_some(asio::buffer(OutgoingPackets.Front()->GetData(), OutgoingPackets.Front()->GetHeader().PacketSize), [this](const asio::error_code& error, std::size_t /*bytesTransferred*/)
    {
        if (!error)
        {
            OutgoingPackets.Pop();
            if (OutgoingPackets.GetSize() > 0)
            {
                WritePacketHeaderAsync();
            }
        }
    });
}

std::shared_ptr<PacketData> ClientNetworkManager::TranslatePacket()
{
    switch (CurrentPacket.GetHeader().PacketType)
    {
    case EPacketType::WorldTime:
        return std::make_shared<WorldTimePacket>(CurrentPacket);
    case EPacketType::EntityData:
        return std::make_shared<EntityDataPacket>(CurrentPacket);
    case EPacketType::ChunkData:
        return std::make_shared<ChunkDataPacket>(CurrentPacket);
    case EPacketType::LightsData:
        return std::make_shared<LightDataPacket>(CurrentPacket);
    case EPacketType::WorldData:
        return std::make_shared<WorldDataPacket>(CurrentPacket);
    case EPacketType::PlayerRotationChange:
        return std::make_shared<PlayerRotateChangePacket>(CurrentPacket);
    default:
        return nullptr;
    }
}

ClientNetworkManager::ClientNetworkManager() : Socket(Context), CurrentPacket(PacketHeader(EPacketType::EntityData))
{
    HeaderBuffer.resize(sizeof(PacketHeader));
}

ClientNetworkManager::~ClientNetworkManager()
{
    Socket.close();
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
    ReadPacketHeaderAsync();
}

std::shared_ptr<PacketData> ClientNetworkManager::GetNextPacket()
{
    if (Packets.GetSize() > 0)
    {
        return Packets.Pop();
    }
    return nullptr;
}

void ClientNetworkManager::WritePacket(const std::shared_ptr<Packet>& packet)
{
    post(Context, [this, packet]
    {
        const bool isEmpty = OutgoingPackets.GetSize() == 0;
        OutgoingPackets.Push(packet);
        if (isEmpty)
        {
            WritePacketHeaderAsync();
        }
    });
}
