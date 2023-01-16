#pragma once
#include "PacketData.h"

class WorldTimePacket final : public PacketData
{
    float NewWorldTime;

public:
    explicit WorldTimePacket(Packet& packet);
};
