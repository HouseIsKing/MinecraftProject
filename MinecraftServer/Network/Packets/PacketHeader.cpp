#include "PacketHeader.h"

#include "World/Blocks/BlockTypeList.h"
#include "World/Chunk.h"

PacketHeader::PacketHeader(const EPacketType packetType) : PacketHeader(packetType, 0)
{
}

PacketHeader::PacketHeader(const EPacketType packetType, const uint32_t packetSize) : PacketType(packetType), PacketSize(packetSize)
{
}

std::array<uint8_t, sizeof(PacketHeader)> PacketHeader::Serialize() const
{
    std::array<uint8_t, sizeof(PacketHeader)> result{};
    memcpy(result.data(), this, sizeof(PacketHeader));
    return result;
}

const PacketHeader PacketHeader::ENTITY_DATA_PACKET = {EPacketType::EntityData, sizeof(float) * 6 + sizeof(uint16_t)};
const PacketHeader PacketHeader::WORLD_TIME_PACKET = {EPacketType::WorldTime, sizeof(long)};
const PacketHeader PacketHeader::PLAYER_ID_PACKET = {EPacketType::PlayerId, sizeof(uint16_t)};
const PacketHeader PacketHeader::CHUNK_DATA_PACKET = {EPacketType::ChunkData, sizeof(int) * 3 + sizeof(EBlockType) * Chunk::CHUNK_WIDTH * Chunk::CHUNK_DEPTH * Chunk::CHUNK_HEIGHT};
const PacketHeader PacketHeader::LIGHTS_DATA_PACKET = {EPacketType::LightsData, sizeof(int) * 2 + sizeof(uint8_t)};