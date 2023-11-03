#pragma once
#include "ConnectionToClientInterface.h"
#include "Network/Packets/Packet.h"
#include "Network/Packets/PacketData.h"
#include "Util/ThreadSafeQueue.h"
#include <asio/ip/tcp.hpp>

class ServerNetworkManager;

class ConnectionToClient final : public ConnectionToClientInterface, public std::enable_shared_from_this<ConnectionToClient>
{
    std::unique_ptr<asio::ip::tcp::socket> Socket;
    ServerNetworkManager* NetworkManager;
    Packet CurrentPacket;
    std::vector<uint8_t> HeaderBuffer{};
    ThreadSafeQueue<Packet> OutgoingPackets;

    std::shared_ptr<ConnectionToClient> GetSharedPtr();
    std::shared_ptr<PacketData> TranslatePacket();
    void ReadPacketBodyAsync();
    void ReadPacketHeaderAsync();
    void WritePacketHeaderAsync();
    void WritePacketBodyAsync();

public:
    ConnectionToClient(asio::io_context& ioContext, ServerNetworkManager* networkManager);
    ~ConnectionToClient() override;
    ConnectionToClient(const ConnectionToClient&) = delete;
    ConnectionToClient& operator=(const ConnectionToClient&) = delete;
    ConnectionToClient(ConnectionToClient&&) = delete;
    ConnectionToClient& operator=(ConnectionToClient&&) = delete;
    asio::ip::tcp::socket& GetSocket() const;
    size_t LastTickPacketReceived{0};

    bool operator==(const ConnectionToClient& other) const
    {
        return ClientName == other.ClientName;
    }

    void Start();
    void WritePacket(const std::shared_ptr<Packet>& packet, const std::shared_ptr<ConnectionToClient>& thisPointer) const;
};
