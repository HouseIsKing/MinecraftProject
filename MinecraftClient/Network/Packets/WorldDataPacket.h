#pragma once
#include "Network/Packets/Packet.h"
#include "PacketData.h"

class WorldDataPacket final : public PacketData
{
    std::vector<std::vector<uint8_t>> Data;
    uint64_t LastInputIdServerProcessed{};

public:
    explicit WorldDataPacket(Packet& packet);
    [[nodiscard]] EPacketType GetPacketType() const override;
    [[nodiscard]] uint64_t GetLastInputIdServerProcessed() const;
    [[nodiscard]] const std::vector<std::vector<uint8_t>>& GetData() const;
};
