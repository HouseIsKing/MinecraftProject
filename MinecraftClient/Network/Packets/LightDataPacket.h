#pragma once
#include "PacketData.h"

class LightDataPacket final : public PacketData
{
    uint8_t Light{};
    int X{};
    int Z{};

public:
    explicit LightDataPacket(Packet& packet);
    [[nodiscard]] uint8_t GetLight() const;
    [[nodiscard]] int GetX() const;
    [[nodiscard]] int GetZ() const;
};
