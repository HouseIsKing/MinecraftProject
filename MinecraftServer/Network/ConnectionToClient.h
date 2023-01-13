#pragma once
#include <asio/ip/tcp.hpp>

#include "ConnectionToClientInterface.h"
#include "Entities/Player.h"
#include "Packets/ConnectionPacket.h"
#include "Packets/PacketData.h"
#include "Packets/PacketHeader.h"

class ClientNetworkManager;

class ConnectionToClient final : public ConnectionToClientInterface
{
    std::unique_ptr<asio::ip::tcp::socket> Socket;
    ClientNetworkManager* NetworkManager;
    ConnectionPacket CurrentPacket;
    std::vector<uint8_t> HeaderBuffer{};

public:
    ConnectionToClient(asio::io_context& ioContext, ClientNetworkManager* networkManager);
    ~ConnectionToClient() override;
    ConnectionToClient(const ConnectionToClient&) = delete;
    ConnectionToClient& operator=(const ConnectionToClient&) = delete;
    ConnectionToClient(ConnectionToClient&&) = delete;
    ConnectionToClient& operator=(ConnectionToClient&&) = delete;

    asio::ip::tcp::socket& GetSocket() const;

    bool operator==(const ConnectionToClient& other) const
    {
        return RepresentingPlayer.GetName() == other.RepresentingPlayer.GetName();
    }

    void Start();
    void ReadPacketBodyAsync();
    void ReadPacketHeaderAsync();
    static std::shared_ptr<PacketData> TranslatePacket(ConnectionPacket& packet);
    //void WritePacketAsync(const PacketHeader& packet);
};
