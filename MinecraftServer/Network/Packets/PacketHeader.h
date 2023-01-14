#pragma once
#include <array>
#include <cstdint>

enum class EPacketType : uint8_t
{
    PlayerId,
    EntityPosition,
    EntityRotation,
    WorldTime
};

struct PacketHeader
{
    EPacketType PacketType;
    uint16_t PacketSize;
    static const PacketHeader PLAYER_ROTATION_PACKET{EPacketType::EntityPosition, sizeof(float) * 3 + sizeof(uint16_t)};
    static const PacketHeader PLAYER_POSITION_PACKET{EPacketType::EntityRotation, sizeof(float) * 3 + sizeof(uint16_t)};
    static const PacketHeader WORLD_TIME_PACKET{EPacketType::WorldTime, sizeof(long)};
    static const PacketHeader PLAYER_ID_PACKET{EPacketType::PlayerId, sizeof(uint16_t)};
    explicit PacketHeader(EPacketType packetType);
    PacketHeader(EPacketType packetType, uint16_t packetSize);
    std::array<uint8_t, 4> Serialize() const;
};
