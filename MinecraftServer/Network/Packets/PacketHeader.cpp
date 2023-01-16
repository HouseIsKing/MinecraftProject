#include "PacketHeader.h"

#include "World/Chunk.h"
#include "World/Blocks/BlockTypeList.h"

PacketHeader::PacketHeader(const EPacketType packetType) : PacketHeader(packetType, 0)
{
}

PacketHeader::PacketHeader(const EPacketType packetType, const uint16_t packetSize) : PacketType(packetType), PacketSize(packetSize)
{
}

std::array<uint8_t, sizeof(PacketHeader)> PacketHeader::Serialize() const
{
    std::array<uint8_t, sizeof(PacketHeader)> result{};
    memcpy(result.data(), this, sizeof(PacketHeader));
    return result;
}

const PacketHeader PacketHeader::PLAYER_ROTATION_PACKET = {EPacketType::EntityPosition, sizeof(float) * 3 + sizeof(uint16_t)};
const PacketHeader PacketHeader::PLAYER_POSITION_PACKET = {EPacketType::EntityRotation, sizeof(float) * 3 + sizeof(uint16_t)};
const PacketHeader PacketHeader::WORLD_TIME_PACKET = {EPacketType::WorldTime, sizeof(long)};
const PacketHeader PacketHeader::PLAYER_ID_PACKET = {EPacketType::PlayerId, sizeof(uint16_t)};
const PacketHeader PacketHeader::CHUNK_POSITION_PACKET = {EPacketType::ChunkPosition, sizeof(int) * 3};
const PacketHeader PacketHeader::CHUNK_DATA_PACKET = {EPacketType::ChunkData, sizeof(EBlockType) * Chunk::CHUNK_WIDTH * Chunk::CHUNK_DEPTH * Chunk::CHUNK_HEIGHT};