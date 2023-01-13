#include "ConnectionToClient.h"

#include <iostream>

#include "ClientNetworkManager.h"
#include "Packets/ConnectionInitPacket.h"

ConnectionToClient::ConnectionToClient(asio::io_context& ioContext, ClientNetworkManager* networkManager) : Socket(std::make_unique<asio::ip::tcp::socket>(ioContext)), NetworkManager(networkManager), CurrentPacket(this, PacketHeader(EPacketType::ConnectionInit))
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
    PacketHeader header = *reinterpret_cast<PacketHeader*>(HeaderBuffer.data());
    CurrentPacket = {this, header};
    std::vector<uint8_t>& data = CurrentPacket.ThePacket.GetData();
    data.resize(CurrentPacket.ThePacket.GetHeader().PacketSize);
    Socket->read_some(asio::buffer(data, CurrentPacket.ThePacket.GetHeader().PacketSize));
    const std::shared_ptr<ConnectionInitPacket> packet = std::dynamic_pointer_cast<ConnectionInitPacket>(TranslatePacket(CurrentPacket));
    RepresentingPlayer = Player(packet->GetClientName());
}

void ConnectionToClient::ReadPacketBodyAsync()
{
    Socket->async_read_some(asio::buffer(CurrentPacket.ThePacket.GetData(), CurrentPacket.ThePacket.GetHeader().PacketSize), [this](const asio::error_code& error, std::size_t
                            /*length*/)
                            {
                                if (error)
                                {
                                    std::cout << "Error reading packet body: " << error.message() << std::endl;
                                    return;
                                }
                                NetworkManager->AddPacket(TranslatePacket(CurrentPacket));
                                ReadPacketHeaderAsync();
                            });
}

void ConnectionToClient::ReadPacketHeaderAsync()
{
    Socket->async_read_some(asio::buffer(HeaderBuffer, HeaderBuffer.size()), [this](const std::error_code ec, std::size_t /*length*/)
    {
        if (!ec)
        {
            PacketHeader header = *reinterpret_cast<PacketHeader*>(HeaderBuffer.data());
            CurrentPacket = {this, header};
            ReadPacketBodyAsync();
        }
        else
        {
            std::cout << "Error reading packet header: " << ec.message() << std::endl;
        }
    });
}

std::shared_ptr<PacketData> ConnectionToClient::TranslatePacket(ConnectionPacket& packet)
{
    switch (packet.ThePacket.GetHeader().PacketType)
    {
    case EPacketType::ConnectionInit:
        return std::make_shared<ConnectionInitPacket>(packet);
    case EPacketType::Position:
        return nullptr;
    }
    return nullptr;
}
