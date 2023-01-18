#pragma once
#include <memory>

class ConnectionToClient;

class PacketData
{
protected:
    std::shared_ptr<ConnectionToClient> Source;
    explicit PacketData(std::shared_ptr<ConnectionToClient> source);
    virtual ~PacketData() = default;

public:
    const std::shared_ptr<ConnectionToClient>& GetConnectionToClient() const;
    PacketData(const PacketData&) = delete;
    PacketData& operator=(const PacketData&) = delete;
    PacketData(PacketData&&) = delete;
    PacketData& operator=(PacketData&&) = delete;
};
