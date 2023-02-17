#include "ClientNetworkManager.h"
#include <iostream>
#include "Packets/WorldDataPacket.h"
#include "IncludeSorter.h"
#include "Packets/PlayerIdPacket.h"

void ClientNetworkManager::ReadPacketBodyAsync()
{
    async_read(Socket, asio::buffer(CurrentPacket.GetData(), CurrentPacket.GetHeader().PacketSize), [this](const asio::error_code& error, const std::size_t
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
    async_read(Socket, asio::buffer(HeaderBuffer, HeaderBuffer.size()), [this](const std::error_code ec, const std::size_t /*length*/)
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
    async_write(Socket, asio::buffer(OutgoingPackets.Front()->GetHeader().Serialize(), sizeof(PacketHeader)), [this](const asio::error_code& error, std::size_t /*bytesTransferred*/)
    {
        if (!error)
        {
            WritePacketBodyAsync();
        }
    });
}

void ClientNetworkManager::WritePacketBodyAsync()
{
    async_write(Socket, asio::buffer(OutgoingPackets.Front()->GetData(), OutgoingPackets.Front()->GetHeader().PacketSize), [this](const asio::error_code& error, std::size_t /*bytesTransferred*/)
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
    case EPacketType::WorldData:
        return std::make_shared<WorldDataPacket>(CurrentPacket);
    case EPacketType::PlayerId:
        return std::make_shared<PlayerIdPacket>(CurrentPacket);
    default:
        return nullptr;
    }
}

ClientNetworkManager::ClientNetworkManager() : Socket(Context), CurrentPacket(PacketHeader(EPacketType::ClientInput))
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
