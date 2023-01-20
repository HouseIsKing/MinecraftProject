#pragma once
#include "EntityDataPacket.h"
#include "PacketData.h"

enum class EEntityType : uint8_t;

class EntitySpawnPacket final : public PacketData
{
    EEntityType EntityType;
    EntityDataPacket EntityData;
    static uint8_t HelperFunc(Packet& packet);

public:
    explicit EntitySpawnPacket(Packet& packet);
    [[nodiscard]] EEntityType GetEntityType() const;
    [[nodiscard]] const EntityDataPacket& GetEntityData() const;
    [[nodiscard]] EPacketType GetPacketType() const override;
};
