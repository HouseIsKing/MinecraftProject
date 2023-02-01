#include "WorldDataPacket.h"

WorldDataPacket::WorldDataPacket(Packet& packet) : WorldTime(0)
{
    Data.resize(packet.GetData().size() - sizeof(uint64_t));
    for (uint8_t& i : Data)
    {
        packet >> i;
    }
    packet >> WorldTime;
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
