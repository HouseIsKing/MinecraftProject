#pragma once
#include "PacketData.h"

class EntityRemovedPacket final : public PacketData
{
    uint16_t Id{};

public:
    explicit EntityRemovedPacket(Packet& packet);
    [[nodiscard]] EPacketType GetPacketType() const override;
    [[nodiscard]] uint16_t GetId() const;
};
