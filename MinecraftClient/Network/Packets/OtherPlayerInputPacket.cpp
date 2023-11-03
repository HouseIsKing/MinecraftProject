#include "OtherPlayerInputPacket.h"

OtherPlayerInputPacket::OtherPlayerInputPacket(Packet& packet)
{
    packet >> TickIndex >> Input >> PlayerId;
}

EPacketType OtherPlayerInputPacket::GetPacketType() const
{
    return EPacketType::ClientInput;
}

uint16_t OtherPlayerInputPacket::GetPlayerId() const
{
    return PlayerId;
}

uint64_t OtherPlayerInputPacket::GetTickIndex() const
{
    return TickIndex;
}

const ClientInputStatusStruct& OtherPlayerInputPacket::GetInput() const
{
    return Input;
}
