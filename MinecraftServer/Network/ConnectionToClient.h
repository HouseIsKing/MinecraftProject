#pragma once
#include <asio/ip/tcp.hpp>

#include "ConnectionToClientInterface.h"
#include "Packets/PacketData.h"
#include "Packets/PacketHeader.h"
#include "Util/ThreadSafeQueue.h"

class ServerNetworkManager;

class ConnectionToClient final : public ConnectionToClientInterface, public std::enable_shared_from_this<ConnectionToClient>
{
    std::unique_ptr<asio::ip::tcp::socket> Socket;
    ServerNetworkManager* NetworkManager;
    Packet CurrentPacket;
    std::vector<uint8_t> HeaderBuffer{};

    std::shared_ptr<ConnectionToClient> GetSharedPtr();
    std::shared_ptr<PacketData> TranslatePacket();
    void ReadPacketBodyAsync();
    void ReadPacketHeaderAsync();

public:
    ConnectionToClient(asio::io_context& ioContext, ServerNetworkManager* networkManager);
    ~ConnectionToClient() override;
    ConnectionToClient(const ConnectionToClient&) = delete;
    ConnectionToClient& operator=(const ConnectionToClient&) = delete;
    ConnectionToClient(ConnectionToClient&&) = delete;
    ConnectionToClient& operator=(ConnectionToClient&&) = delete;

    asio::ip::tcp::socket& GetSocket() const;

    bool operator==(const ConnectionToClient& other) const
    {
        return ClientName == other.ClientName;
    }

    void Start();
    void WritePacket(Packet& packet) const;
};
