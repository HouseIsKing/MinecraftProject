#include "PlayerRotateChangePacket.h"

PlayerRotateChangePacket::PlayerRotateChangePacket(Packet& packet)
{
    packet >> X >> Y >> Z;
}

float PlayerRotateChangePacket::GetX() const
{
    return X;
}

float PlayerRotateChangePacket::GetY() const
{
    return Y;
}

float PlayerRotateChangePacket::GetZ() const
{
    return Z;
}
