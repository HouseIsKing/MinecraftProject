#pragma once
#include "BlockParticleEntityState.h"
#include "EntityStateWrapper.h"

class BlockParticleEntityStateWrapper final : public EntityStateWrapper<BlockParticleEntityState>
{
    void WriteChangesToVector(std::vector<uint8_t>& changes, const EChangeTypeEntity& changeType) const override;
    void ClearAllChanges(const EChangeTypeEntity& change) override;

public:
    explicit BlockParticleEntityStateWrapper(uint16_t id);
    explicit BlockParticleEntityStateWrapper(const BlockParticleEntityState& otherState);
    void WriteChangesToVector(std::vector<uint8_t>& changes) const override;
    void ClearAllChanges() override;
    void SetLifeTime(uint8_t lifeTime);
    void SetAge(uint8_t age);
    void SetBlockType(EBlockType blockType);
};
