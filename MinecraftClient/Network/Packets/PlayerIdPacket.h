#pragma once

#include "PacketData.h"

class PlayerIdPacket final : public PacketData
{
    uint16_t Id;

public:
    explicit PlayerIdPacket(Packet& packet);
};
