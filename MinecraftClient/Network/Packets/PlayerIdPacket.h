#pragma once
#include "PacketData.h"

class PlayerIdPacket final : public PacketData
{
    uint16_t PlayerId;

public:
    explicit PlayerIdPacket(Packet& packet);
    [[nodiscard]] EPacketType GetPacketType() const override;
    [[nodiscard]] uint16_t GetPlayerId() const;
};
