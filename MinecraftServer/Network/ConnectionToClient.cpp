#include "ConnectionToClient.h"
#include "ServerNetworkManager.h"
#include <iostream>

ConnectionToClient::ConnectionToClient(asio::io_context& ioContext, ServerNetworkManager* networkManager) : Socket(std::make_unique<asio::ip::tcp::socket>(ioContext)), NetworkManager(networkManager), CurrentPacket(PacketHeader(EPacketType::PlayerId))
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
    Socket->async_wait(asio::socket_base::wait_error, [this](const asio::error_code& error)
    {
        if (error)
        {
            std::cout << "Client " << ClientName << " disconnected" << std::endl;
            NetworkManager->AddRemovedConnection(GetSharedPtr());
        }
    });
    ReadPacketHeaderAsync();
}

void ConnectionToClient::WritePacket(Packet& packet) const
{
    Socket->async_write_some(asio::buffer(packet.GetHeader().Serialize(), sizeof(PacketHeader)), [this](const asio::error_code& error, std::size_t /*bytesTransferred*/)
    {
        if (error)
        {
            std::cout << "Error writing packet header to client " << ClientName << std::endl;
        }
    });
    Socket->async_write_some(asio::buffer(packet.GetData(), packet.GetHeader().PacketSize), [this](const asio::error_code& error, std::size_t /*bytesTransferred*/)
    {
        if (error)
        {
            std::cout << "Error writing packet data to client " << ClientName << std::endl;
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
        else
        {
            std::cout << "Error reading packet header: " << ec.message() << std::endl;
        }
    });
}

std::shared_ptr<PacketData> ConnectionToClient::TranslatePacket()
{
    switch (CurrentPacket.GetHeader().PacketType)
    {
    case EPacketType::PlayerId:
        return nullptr;
    }
    return nullptr;
}
