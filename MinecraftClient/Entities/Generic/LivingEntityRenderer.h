#pragma once
#include "EntityRenderer.h"
#include "Util/States/LivingEntityState.h"

class LivingEntityRenderer : public EntityRenderer
{
protected:
    explicit LivingEntityRenderer(uint16_t entityId);
};
