#pragma once
#include "EntityRenderer.h"
#include "Util/EngineDefaults.h"
#include "Util/States/BlockParticleEntityState.h"

class BlockParticleEntityRenderer final : public EntityRenderer<BlockParticleEntityState>
{
    float U0;
    float V0;
    int PreviousLightLevel;
    void GenerateTextureTessellation(int lightLevel);

public:
    BlockParticleEntityRenderer(const BlockParticleEntityState& state, const BlockParticleEntityState& oldState);
    void Render(float partialTick) override;
};
