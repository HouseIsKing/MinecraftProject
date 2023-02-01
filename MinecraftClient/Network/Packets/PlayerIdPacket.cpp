#include "PlayerIdPacket.h"

PlayerIdPacket::PlayerIdPacket(Packet& packet) : PlayerId(0)
{
    packet >> PlayerId;
}

EPacketType PlayerIdPacket::GetPacketType() const
{
    return EPacketType::PlayerId;
}

uint16_t PlayerIdPacket::GetPlayerId() const
{
    return PlayerId;
}
