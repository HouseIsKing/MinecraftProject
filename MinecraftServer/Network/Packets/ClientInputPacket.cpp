#include "ClientInputPacket.h"

ClientInputPacket::ClientInputPacket(Packet& packet, std::shared_ptr<ConnectionToClient> connection) : PacketData(
    std::move(connection))
{
    packet >> WorldTickSent;
    packet >> State;
}

const ClientInputStatusStruct& ClientInputPacket::GetState() const
{
    return State;
}

uint64_t ClientInputPacket::GetWorldTickSent() const
{
    return WorldTickSent;
}

EPacketType ClientInputPacket::GetPacketType() const
{
    return EPacketType::ClientInput;
}
