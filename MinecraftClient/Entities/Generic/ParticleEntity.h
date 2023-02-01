#pragma once
#include "Entity.h"
#include "Util/EngineDefaults.h"

class Block;

class ParticleEntity final : public Entity
{
    static constexpr glm::vec3 PARTICLE_SIZE = glm::vec3(0.1F, 0.1F, 0.1F);
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