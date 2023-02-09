#pragma once
#include "Network/Packets/Packet.h"
#include "PacketData.h"

class WorldDataPacket final : public PacketData
{
    uint64_t WorldTime;
    std::vector<uint8_t> Data;

public:
    explicit WorldDataPacket(Packet& packet);
    [[nodiscard]] EPacketType GetPacketType() const override;
    [[nodiscard]] uint64_t GetWorldTime() const;
    [[nodiscard]] const std::vector<uint8_t>& GetData() const;
};
