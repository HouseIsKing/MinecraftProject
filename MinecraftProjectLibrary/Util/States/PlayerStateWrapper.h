#pragma once
#include "LivingEntityStateWrapper.h"
#include "PlayerState.h"

class PlayerStateWrapper final : public LivingEntityStateWrapper<PlayerState>
{
protected:
    void WriteChangesToVector(std::vector<uint8_t>& changes, const EChangeTypeEntity& changeType) const override;
    void ClearAllChanges(const EChangeTypeEntity& changeType) override;

public:
    explicit PlayerStateWrapper(uint16_t id);
    explicit PlayerStateWrapper(const PlayerState& otherState);
    void SetInputState(const ClientInputStruct& inputState);
    void SetCameraPitch(float cameraPitch);
    void SetMode(bool mode);
    void SetCurrentSelectedBlock(EBlockType currentSelectedBlock);
    void WriteChangesToVector(std::vector<uint8_t>& changes) const override;
    void ClearAllChanges() override;
};
