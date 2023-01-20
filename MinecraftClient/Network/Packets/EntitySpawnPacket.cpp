#include "EntitySpawnPacket.h"

uint8_t EntitySpawnPacket::HelperFunc(Packet& packet)
{
    uint8_t temp;
    packet >> temp;
    return temp;
}

EntitySpawnPacket::EntitySpawnPacket(Packet& packet) : EntityType(static_cast<EEntityType>(HelperFunc(packet))),
                                                       EntityData(packet)
{
}

EEntityType EntitySpawnPacket::GetEntityType() const
{
    return EntityType;
}

const EntityDataPacket& EntitySpawnPacket::GetEntityData() const
{
    return EntityData;
}

EPacketType EntitySpawnPacket::GetPacketType() const
{
    return EPacketType::EntityEnterWorld;
}
