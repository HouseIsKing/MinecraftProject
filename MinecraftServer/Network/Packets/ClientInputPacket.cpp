#include "ClientInputPacket.h"

ClientInputPacket::ClientInputPacket(Packet& packet, std::shared_ptr<ConnectionToClient> connection) : PacketData(
    std::move(connection))
{
    packet >> InputId;
    packet >> State;
}

const ClientInputStatusStruct& ClientInputPacket::GetState() const
{
    return State;
}

uint64_t ClientInputPacket::GetInputId() const
{
    return InputId;
}

EPacketType ClientInputPacket::GetPacketType() const
{
    return EPacketType::ClientInput;
}
