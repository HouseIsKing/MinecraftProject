#include "PacketHeader.h"

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

const PacketHeader PacketHeader::KEYBOARD_PACKET = PacketHeader(EPacketType::Keyboard, 2 * sizeof(int));
const PacketHeader PacketHeader::MOUSE_POS_PACKET = PacketHeader(EPacketType::MousePosition, 2 * sizeof(float));
const PacketHeader PacketHeader::MOUSE_BUTTON_PACKET = PacketHeader(EPacketType::MouseButton, 2 * sizeof(int));