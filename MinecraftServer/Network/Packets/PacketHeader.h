#pragma once
#include <array>
#include <cstdint>

enum class EPacketType : uint8_t
{
    PlayerId,
    EntityPosition,
    EntityRotation,
    ChunkPosition,
    ChunkData,
    WorldTime
};

struct PacketHeader
{
    EPacketType PacketType;
    uint16_t PacketSize;
    static const PacketHeader PLAYER_ROTATION_PACKET;
    static const PacketHeader PLAYER_POSITION_PACKET;
    static const PacketHeader WORLD_TIME_PACKET;
    static const PacketHeader PLAYER_ID_PACKET;
    static const PacketHeader CHUNK_POSITION_PACKET;
    static const PacketHeader CHUNK_DATA_PACKET;
    explicit PacketHeader(EPacketType packetType);
    PacketHeader(EPacketType packetType, uint16_t packetSize);
    [[nodiscard]] std::array<uint8_t, 4> Serialize() const;
};
