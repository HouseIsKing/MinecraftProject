#pragma once
#include "Packet.h"

class ConnectionToClient;

class PacketData
{
    std::string ClientName;

protected:
    explicit PacketData(std::string name);
    PacketData() = default;
    ~PacketData() = default;

public:
    PacketData(const PacketData&) = delete;
    PacketData& operator=(const PacketData&) = delete;
    PacketData(PacketData&&) = delete;
    PacketData& operator=(PacketData&&) = delete;
    virtual Packet GetPacketAsSend();
    const std::string& GetClientName();
};
