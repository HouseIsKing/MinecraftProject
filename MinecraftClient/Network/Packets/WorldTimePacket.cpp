#include "WorldTimePacket.h"

WorldTimePacket::WorldTimePacket(Packet& packet)
{
    packet >> NewWorldTime >> TicksTimer;
}

long WorldTimePacket::GetNewWorldTime() const
{
    return NewWorldTime;
}

float WorldTimePacket::GetTicksTimer() const
{
    return TicksTimer;
}

EPacketType WorldTimePacket::GetPacketType() const
{
    return EPacketType::WorldTime;
}
