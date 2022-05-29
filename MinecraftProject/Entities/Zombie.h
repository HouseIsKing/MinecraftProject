#pragma once
#include "EntityMeshBlock.h"
#include "LivingEntity.h"

class Zombie final : public LivingEntity
{
    constexpr static vec3 ZOMBIE_SIZE = vec3(0.3F, 0.9F, 0.3F);
    constexpr static float ZOMBIE_SCALE_FACTOR = 0.05833334F;
    const float TimeOffset;
    int PreviousLightLevel;
    float NextRotation;
    EntityMeshBlock Head;
    EntityMeshBlock Body;
    EntityMeshBlock RightArm;
    EntityMeshBlock LeftArm;
    EntityMeshBlock RightLeg;
    EntityMeshBlock LeftLeg;
    void RebuildRender(int brightness);
public:
    Zombie(uint16_t entityId, float x, float y, float z);
    void Render() override;
    void Tick() override;
};
