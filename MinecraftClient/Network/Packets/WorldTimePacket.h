#pragma once
#include "PacketData.h"

class WorldTimePacket final : public PacketData
{
    long NewWorldTime{};
    float TicksTimer{};

public:
    explicit WorldTimePacket(Packet& packet);
    [[nodiscard]] long GetNewWorldTime() const;
    [[nodiscard]] float GetTicksTimer() const;
};
