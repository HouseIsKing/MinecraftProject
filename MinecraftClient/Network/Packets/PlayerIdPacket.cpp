#include "PlayerIdPacket.h"

PlayerIdPacket::PlayerIdPacket(Packet& packet) : Id(0)
{
    packet >> Id;
}
