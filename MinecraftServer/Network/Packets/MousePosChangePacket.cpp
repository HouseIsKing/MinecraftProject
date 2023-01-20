#include "MousePosChangePacket.h"

#include <utility>

MousePosChangePacket::MousePosChangePacket(std::shared_ptr<ConnectionToClient> client, Packet& packetToRead) : PacketData(
    std::move(client)), X(0.0F), Y(0.0F)
{
    packetToRead >> X >> Y;
}

float MousePosChangePacket::GetX() const
{
    return X;
}

float MousePosChangePacket::GetY() const
{
    return Y;
}

EPacketType MousePosChangePacket::GetPacketType() const
{
    return EPacketType::MousePosition;
}
