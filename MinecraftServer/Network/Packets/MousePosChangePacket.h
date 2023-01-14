#pragma once
#include "PacketData.h"

class MousePosChangePacket final : PacketData
{
    float X;
    float Y;

public:
    MousePosChangePacket(std::shared_ptr<ConnectionToClient> client, Packet& packetToRead);
    float GetX() const;
    float GetY() const;
};
