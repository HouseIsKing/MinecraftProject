#pragma once
#include "Entity.h"
#include "Util/States/BlockParticleEntityStateWrapper.h"

class BlockParticleEntity final : public Entity<BlockParticleEntityStateWrapper, BlockParticleEntityState>
{
public:
    explicit BlockParticleEntity(const BlockParticleEntityState& state);
    void ApplyRevertEntityChange(const std::vector<uint8_t>& changes, size_t& pos, EChangeTypeEntity change, bool revert) override;
    void Tick() override;
};