#include "EntityRemovedPacket.h"

EntityRemovedPacket::EntityRemovedPacket(Packet& packet)
{
    packet >> Id;
}

EPacketType EntityRemovedPacket::GetPacketType() const
{
    return EPacketType::EntityLeaveWorld;
}

uint16_t EntityRemovedPacket::GetId() const
{
    return Id;
}
