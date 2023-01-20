#pragma once
#include "Entities/Generic/LivingEntity.h"
#include "World/MP/MultiPlayerWorld.h"

class OtherPlayer final : public LivingEntity<MultiPlayerWorld>
{
    constexpr static glm::vec3 PLAYER_SIZE = glm::vec3(0.3F, 0.9F, 0.3F);

public:
    OtherPlayer(const float x, const float y, const float z)
        : LivingEntity(PLAYER_SIZE, x, y, z)
    {
    }

    void HandleEntityUpdate(const EntityDataPacket& packet) override;
    [[nodiscard]] EEntityType GetEntityType() const override;
};
