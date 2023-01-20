#include "MouseChangePacket.h"

#include <utility>

MouseChangePacket::MouseChangePacket(std::shared_ptr<ConnectionToClient> client, Packet& packetToRead) : PacketData(
    std::move(client)), Button(0), Action(0)
{
    packetToRead >> Button >> Action;
}

int MouseChangePacket::GetButton() const
{
    return Button;
}

int MouseChangePacket::GetAction() const
{
    return Action;
}

EPacketType MouseChangePacket::GetPacketType() const
{
    return EPacketType::MouseButton;
}
