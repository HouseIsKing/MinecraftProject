#pragma once
#include "Packets/PacketData.h"
#include "Util/ThreadSafeQueue.h"
#include "IncludeSorter.h"
#include <thread>
#include "Network/Packets/Packet.h"

class ClientNetworkManager
{
    asio::io_context Context;
    asio::io_context::work IdleWork{Context};
    asio::ip::tcp::socket Socket;
    Packet CurrentPacket;
    ThreadSafeQueue<PacketData> Packets;
    std::vector<uint8_t> HeaderBuffer{};
    ThreadSafeQueue<Packet> OutgoingPackets;
    std::thread ContextThread = std::thread([&] { Context.run(); });

    void ReadPacketBodyAsync();
    void ReadPacketHeaderAsync();
    void WritePacketHeaderAsync();
    void WritePacketBodyAsync();
    std::shared_ptr<PacketData> TranslatePacket();

public:
    ClientNetworkManager();
    ~ClientNetworkManager();
    ClientNetworkManager(const ClientNetworkManager&) = delete;
    ClientNetworkManager& operator=(const ClientNetworkManager&) = delete;
    ClientNetworkManager(ClientNetworkManager&&) = delete;
    ClientNetworkManager& operator=(ClientNetworkManager&&) = delete;
    void Start(const std::string& ip, const std::string& name);
    std::shared_ptr<PacketData> GetNextPacket();
    size_t GetLastTickPacketReceived() const;
    void WritePacket(const std::shared_ptr<Packet>& packet);
};
