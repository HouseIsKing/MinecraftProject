#pragma once
#include "PacketData.h"
#include <string>

class ConnectionInitPacket final : public PacketData
{
    std::string ClientName;

public:
    explicit ConnectionInitPacket(std::string clientName);
    [[nodiscard]] const std::string& GetClientName() const;
    Packet GetPacketAsSend() override;
};
