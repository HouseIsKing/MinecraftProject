#pragma once
#include "EntityRenderer.h"
#include "Util/EngineDefaults.h"
#include "Util/States/BlockParticleEntityState.h"

class BlockParticleEntityRenderer final : public EntityRenderer
{
    float U0;
    float V0;
    int PreviousLightLevel;
    void GenerateTextureTessellation(int lightLevel);

public:
    explicit BlockParticleEntityRenderer(uint16_t entityId);
    void Render(float partialTick) override;
    const EntityState& GetState() override;
    EEntityType GetEntityTypeRenderer() override;
};
