#pragma once
#include "Entities/Generic/LivingEntityRenderer.h"
#include "Util/States/PlayerState.h"


class PlayerRenderer : public LivingEntityRenderer
{
public:
    explicit PlayerRenderer(uint16_t entityId);
    const EntityState& GetState() override;
    EEntityType GetEntityTypeRenderer() override;
};
