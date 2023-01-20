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
    PlayerRotationChange,
    PlaceBlock,
    DestroyBlock,
    ChangeSelectedBlock,
    EntityEnterWorld,
    EntityLeaveWorld,
    ChangePlayerMode,
};

struct PacketHeader
{
    EPacketType PacketType;
    uint32_t PacketSize;
    static const PacketHeader ENTITY_DATA_PACKET;
    static const PacketHeader PLAYER_ROTATION_CHANGE_PACKET;
    static const PacketHeader WORLD_TIME_PACKET;
    static const PacketHeader PLAYER_ID_PACKET;
    static const PacketHeader CHUNK_DATA_PACKET;
    static const PacketHeader LIGHTS_DATA_PACKET;
    static const PacketHeader CHANGE_SELECTED_BLOCK_PACKET;
    static const PacketHeader PLACE_BLOCK_PACKET;
    static const PacketHeader REMOVE_BLOCK_PACKET;
    static const PacketHeader ENTITY_ENTER_WORLD_PACKET;
    static const PacketHeader ENTITY_LEAVE_WORLD_PACKET;
    static const PacketHeader CHANGE_PLAYER_MODE_PACKET;
    explicit PacketHeader(EPacketType packetType);
    PacketHeader(EPacketType packetType, uint32_t packetSize);
    [[nodiscard]] std::array<uint8_t, 8> Serialize() const;
};
