#pragma once
#include <array>
#include <cstdint>

enum class EPacketType : uint8_t
{
    PlayerId,
    EntityData,
    ChunkData,
    LightsData,
    WorldTime,
    WorldData
};

struct PacketHeader
{
    EPacketType PacketType;
    uint32_t PacketSize;
    static const PacketHeader ENTITY_DATA_PACKET;
    static const PacketHeader WORLD_TIME_PACKET;
    static const PacketHeader PLAYER_ID_PACKET;
    static const PacketHeader CHUNK_DATA_PACKET;
    static const PacketHeader LIGHTS_DATA_PACKET;
    explicit PacketHeader(EPacketType packetType);
    PacketHeader(EPacketType packetType, uint32_t packetSize);
    [[nodiscard]] std::array<uint8_t, 8> Serialize() const;
};
