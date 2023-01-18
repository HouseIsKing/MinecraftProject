#include "ConnectionToClient.h"
#include "ServerNetworkManager.h"
#include <iostream>

#include "Packets/KeyChangePacket.h"
#include "Packets/MouseChangePacket.h"
#include "Packets/MousePosChangePacket.h"

ConnectionToClient::ConnectionToClient(asio::io_context& ioContext, ServerNetworkManager* networkManager) : Socket(std::make_unique<asio::ip::tcp::socket>(ioContext)), NetworkManager(networkManager), CurrentPacket(PacketHeader(EPacketType::EntityData))
{
    HeaderBuffer.resize(sizeof(PacketHeader));
}

ConnectionToClient::~ConnectionToClient()
{
    if (Socket != nullptr)
    {
        Socket->close();
    }
}

asio::ip::tcp::socket& ConnectionToClient::GetSocket() const
{
    return *Socket;
}

void ConnectionToClient::Start()
{
    Socket->read_some(asio::buffer(HeaderBuffer, HeaderBuffer.size()));
    const PacketHeader header = *reinterpret_cast<PacketHeader*>(HeaderBuffer.data());
    CurrentPacket = Packet{header};
    Socket->read_some(asio::buffer(CurrentPacket.GetData(), CurrentPacket.GetHeader().PacketSize));
    CurrentPacket >> ClientName;
    ReadPacketHeaderAsync();
}

void ConnectionToClient::WritePacket(const std::shared_ptr<Packet>& packet)
{
    post(NetworkManager->GetContext(), [this, packet]
    {
        const bool isEmpty = OutgoingPackets.GetSize() == 0;
        OutgoingPackets.Push(packet);
        if (isEmpty)
        {
            WritePacketHeaderAsync();
        }
    });
}

std::shared_ptr<ConnectionToClient> ConnectionToClient::GetSharedPtr()
{
    return shared_from_this();
}

void ConnectionToClient::ReadPacketBodyAsync()
{
    Socket->async_read_some(asio::buffer(CurrentPacket.GetData(), CurrentPacket.GetHeader().PacketSize), [this](const asio::error_code& error, std::size_t
                            /*length*/)
                            {
                                if (!error)
                                {
                                    NetworkManager->AddPacket(TranslatePacket());
                                    ReadPacketHeaderAsync();
                                }
                                else if (error == asio::error::eof || error == asio::error::connection_reset)
                                {
                                    Socket->close();
                                    std::cout << "Client " << ClientName << " disconnected" << std::endl;
                                    NetworkManager->AddRemovedConnection(GetSharedPtr());
                                }
                                else
                                {
                                    std::cout << "Error reading packet body: " << error.message() << std::endl;
                                }
                            });
}

void ConnectionToClient::ReadPacketHeaderAsync()
{
    Socket->async_read_some(asio::buffer(HeaderBuffer, HeaderBuffer.size()), [this](const std::error_code ec, std::size_t /*length*/)
    {
        if (!ec)
        {
            const PacketHeader header = *reinterpret_cast<PacketHeader*>(HeaderBuffer.data());
            CurrentPacket = Packet{header};
            ReadPacketBodyAsync();
        }
        else if (ec == asio::error::eof || ec == asio::error::connection_reset)
        {
            Socket->close();
            std::cout << "Client " << ClientName << " disconnected" << std::endl;
            NetworkManager->AddRemovedConnection(GetSharedPtr());
        }
        else
        {
            std::cout << "Error reading packet header: " << ec.message() << std::endl;
        }
    });
}

void ConnectionToClient::WritePacketHeaderAsync()
{
    Socket->async_write_some(asio::buffer(OutgoingPackets.Front()->GetHeader().Serialize(), sizeof(PacketHeader)), [this](const asio::error_code& error, std::size_t /*bytesTransferred*/)
    {
        if (!error)
        {
            WritePacketBodyAsync();
        }
    });
}

void ConnectionToClient::WritePacketBodyAsync()
{
    Socket->async_write_some(asio::buffer(OutgoingPackets.Front()->GetData(), OutgoingPackets.Front()->GetHeader().PacketSize), [this](const asio::error_code& error, std::size_t /*bytesTransferred*/)
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

std::shared_ptr<PacketData> ConnectionToClient::TranslatePacket()
{
    switch (CurrentPacket.GetHeader().PacketType)
    {
    case EPacketType::Keyboard:
        return std::make_shared<KeyChangePacket>(GetSharedPtr(), CurrentPacket);
    case EPacketType::MousePosition:
        return std::make_shared<MousePosChangePacket>(GetSharedPtr(), CurrentPacket);
    case EPacketType::MouseButton:
        return std::make_shared<MouseChangePacket>(GetSharedPtr(), CurrentPacket);
    default:
        return nullptr;
    }
}
