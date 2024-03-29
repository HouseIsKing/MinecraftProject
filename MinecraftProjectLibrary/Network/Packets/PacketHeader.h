#pragma once
#include "Util/EngineDefaults.h"
#include <cstdint>

struct PacketHeader
{
    EPacketType PacketType;
    uint32_t PacketSize;
    static const PacketHeader PLAYER_ID_PACKET;
    static const PacketHeader CLIENT_INPUT_HEADER;
    explicit PacketHeader(EPacketType packetType);
    PacketHeader(EPacketType packetType, uint32_t packetSize);
    [[nodiscard]] std::array<uint8_t, 8> Serialize() const;
};
