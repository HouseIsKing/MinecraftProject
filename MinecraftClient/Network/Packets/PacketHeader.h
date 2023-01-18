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
    WorldData,
    MousePosition,
    MouseButton,
    Keyboard,
    PlayerRotationChange
};

struct PacketHeader
{
    EPacketType PacketType;
    uint32_t PacketSize;
    explicit PacketHeader(EPacketType packetType);
    PacketHeader(EPacketType packetType, uint32_t packetSize);
    [[nodiscard]] std::array<uint8_t, 8> Serialize() const;

    static const PacketHeader MOUSE_POS_PACKET;
    static const PacketHeader MOUSE_BUTTON_PACKET;
    static const PacketHeader KEYBOARD_PACKET;
};
