#pragma once
#include "ConnectionPacket.h"
#include "PacketData.h"
#include <string>

class ConnectionInitPacket final : public PacketData
{
    std::string ClientName;

public:
    explicit ConnectionInitPacket(ConnectionPacket& packet);
    const std::string& GetClientName() const;
};
