#include "LightDataPacket.h"

LightDataPacket::LightDataPacket(Packet& packet)
{
    packet >> X >> Z >> Light;
}

uint8_t LightDataPacket::GetLight() const
{
    return Light;
}

int LightDataPacket::GetX() const
{
    return X;
}

int LightDataPacket::GetZ() const
{
    return Z;
}

EPacketType LightDataPacket::GetPacketType() const
{
    return EPacketType::LightsData;
}
