#pragma once
#include <array>
#include <cstdint>

enum class EPacketType : uint8_t
{
    ConnectionInit,
    Position
};

struct PacketHeader
{
    EPacketType PacketType;
    uint16_t PacketSize;

    explicit PacketHeader(EPacketType packetType);
    PacketHeader(EPacketType packetType, uint16_t packetSize);
    std::array<uint8_t, 4> Serialize() const;
};
