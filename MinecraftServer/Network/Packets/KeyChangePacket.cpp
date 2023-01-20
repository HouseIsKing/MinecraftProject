#include "KeyChangePacket.h"

#include <utility>

KeyChangePacket::KeyChangePacket(std::shared_ptr<ConnectionToClient> connection, Packet& packetToRead) : PacketData(
    std::move(connection)), Key(0), Action(0)
{
    packetToRead >> Key >> Action;
}

int KeyChangePacket::GetKey() const
{
    return Key;
}

int KeyChangePacket::GetAction() const
{
    return Action;
}

EPacketType KeyChangePacket::GetPacketType() const
{
    return EPacketType::Keyboard;
}
