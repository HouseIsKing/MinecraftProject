#pragma once

class ConnectionToClient;

class PacketData
{
    ConnectionToClient* Source;

protected:
    explicit PacketData(ConnectionToClient* source);
    virtual ~PacketData() = default;

public:
    ConnectionToClient* GetConnectionToClient() const;
    PacketData(const PacketData&) = delete;
    PacketData& operator=(const PacketData&) = delete;
    PacketData(PacketData&&) = delete;
    PacketData& operator=(PacketData&&) = delete;
};
