#pragma once
#include "PacketData.h"
#include "Network/Packets/Packet.h"

class OtherPlayerInputPacket final : public PacketData
{
    uint16_t PlayerId{};
    uint64_t TickIndex{};
    ClientInputStatusStruct Input{};

public:
    explicit OtherPlayerInputPacket(Packet& packet);
    [[nodiscard]] EPacketType GetPacketType() const override;
    [[nodiscard]] uint16_t GetPlayerId() const;
    [[nodiscard]] uint64_t GetTickIndex() const;
    [[nodiscard]] const ClientInputStatusStruct& GetInput() const;
};
