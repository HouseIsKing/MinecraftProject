#pragma once
#include "LivingEntityStateWrapper.h"
#include "PlayerState.h"

class PlayerStateWrapper final : public LivingEntityStateWrapper<PlayerState>
{
public:
    explicit PlayerStateWrapper(uint16_t id);
    explicit PlayerStateWrapper(const PlayerState& otherState);
    void WriteChangesToVector(std::vector<uint8_t>& changes) const override;
    void ClearAllChanges() override;
    void SetInputState(const ClientInputStruct& inputState);
    void SetCameraPitch(float cameraPitch);
};
