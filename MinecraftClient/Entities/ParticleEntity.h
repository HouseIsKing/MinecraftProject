#pragma once
#include "Entity.h"

class ParticleEntity final : Entity
{
    static constexpr vec3 PARTICLE_SIZE = vec3(0.1F, 0.1F, 0.1F);
    float U0;
    float V0;
    const Block* BlockType;
    int PreviousLightLevel;
    const int LifeTime;
    int Age;
    void GenerateTextureTessellation(int lightLevel);

public:
    ParticleEntity(float x, float y, float z, float xSpeed, float ySpeed, float zSpeed, const Block* blockType);
    void Tick() override;
    void Render(float partialTick) override;
};
