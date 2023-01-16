#pragma once
#include "PacketData.h"

class EntityPositionPacket final : public PacketData
{
    uint16_t Id;
    float X;
    float Y;
    float Z;

public:
    explicit EntityPositionPacket(Packet& packet);
};
