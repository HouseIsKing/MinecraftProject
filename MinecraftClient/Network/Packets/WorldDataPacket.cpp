#include "WorldDataPacket.h"

WorldDataPacket::WorldDataPacket(Packet& packet) : Data(packet.GetData()), WorldTime(*reinterpret_cast<uint64_t*>(Data.data() + sizeof uint64_t + sizeof uint16_t + sizeof EChangeType))
{
}

EPacketType WorldDataPacket::GetPacketType() const
{
    return EPacketType::WorldData;
}

uint64_t WorldDataPacket::GetWorldTime() const
{
    return WorldTime;
}

const std::vector<uint8_t>& WorldDataPacket::GetData() const
{
    return Data;
}
