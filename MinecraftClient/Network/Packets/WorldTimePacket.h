#pragma once
#include "PacketData.h"

class WorldTimePacket final : public PacketData
{
    long NewWorldTime;

public:
    explicit WorldTimePacket(Packet& packet);
    [[nodiscard]] long GetNewWorldTime() const;
};
