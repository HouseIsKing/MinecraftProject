#pragma once
#include "Entity.h"
#include "Util/States/LivingEntityState.h"

class LivingEntity : public Entity
{
    void CalculateVelocity() const;

protected:
    LivingEntity(glm::vec3 entitySize, float x, float y, float z, LivingEntityState* state);
    void Tick() override;
    [[nodiscard]] EntityState* GetEntityState() const override;
    void ApplyEntityChange(const std::vector<uint8_t>& changes, size_t& pos, EChangeTypeEntity change) override;
};
