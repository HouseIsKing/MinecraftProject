#pragma once
#include "PacketData.h"

class PlayerRotateChangePacket final : public PacketData
{
    float X{};
    float Y{};
    float Z{};

public:
    explicit PlayerRotateChangePacket(Packet& packet);
    [[nodiscard]] float GetX() const;
    [[nodiscard]] float GetY() const;
    [[nodiscard]] float GetZ() const;
};
