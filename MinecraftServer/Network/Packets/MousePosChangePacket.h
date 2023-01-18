#pragma once
#include "Packet.h"
#include "PacketData.h"

class MousePosChangePacket final : PacketData
{
    float X;
    float Y;

public:
    MousePosChangePacket(std::shared_ptr<ConnectionToClient> client, Packet& packetToRead);
    [[nodiscard]] float GetX() const;
    [[nodiscard]] float GetY() const;
};
