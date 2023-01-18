#include "WorldTimePacket.h"

WorldTimePacket::WorldTimePacket(Packet& packet) : NewWorldTime(0)
{
    packet >> NewWorldTime;
}

long WorldTimePacket::GetNewWorldTime() const
{
    return NewWorldTime;
}
