#include "WorldTimePacket.h"

WorldTimePacket::WorldTimePacket(Packet& packet) : NewWorldTime(0)
{
    packet >> NewWorldTime;
}
