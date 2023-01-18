#include "EntityDataPacket.h"

EntityDataPacket::EntityDataPacket(Packet& packet)
{
    packet >> Id >> XPos >> YPos >> ZPos >> XRot >> YRot >> ZRot;
}

uint16_t EntityDataPacket::GetId() const
{
    return Id;
}

float EntityDataPacket::GetXPos() const
{
    return XPos;
}

float EntityDataPacket::GetYPos() const
{
    return YPos;
}

float EntityDataPacket::GetZPos() const
{
    return ZPos;
}

float EntityDataPacket::GetXRot() const
{
    return XRot;
}

float EntityDataPacket::GetYRot() const
{
    return YRot;
}

float EntityDataPacket::GetZRot() const
{
    return ZRot;
}
