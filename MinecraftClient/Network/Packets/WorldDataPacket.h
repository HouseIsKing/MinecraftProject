#pragma once
#include "Network/Packets/Packet.h"
#include "PacketData.h"

class WorldDataPacket final : public PacketData
{
    std::vector<uint8_t> Data;
    uint64_t WorldTime{};

public:
    explicit WorldDataPacket(Packet& packet);
    [[nodiscard]] EPacketType GetPacketType() const override;
    [[nodiscard]] uint64_t GetWorldTime() const;
    [[nodiscard]] const std::vector<uint8_t>& GetData() const;
};
