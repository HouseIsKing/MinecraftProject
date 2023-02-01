#include "PacketHeader.h"
#include "Entities/Entity.h"
#include "Util/EngineDefaults.h"
#include "World/Blocks/BlockTypeList.h"

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

const PacketHeader PacketHeader::PLAYER_ID_PACKET = {EPacketType::PlayerId, sizeof(uint16_t)};