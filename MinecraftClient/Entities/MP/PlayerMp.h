#pragma once
/*#include "Entities/Generic/ParticleEntity.h"
#include "Entities/SP/PlayerController.h"
#include "Util/States/PlayerState.h"
#include "World/MP/MultiPlayerWorld.h"

class PlayerMp final : public PlayerController
{
public:
    explicit PlayerMp(const PlayerState& state);
    void Tick() override;
    [[nodiscard]] const ClientInputState& GetInputState(size_t tickOffset) const;
    void CopyInputStateToHistory(size_t tickOffset);
    [[nodiscard]] EntityState* GetEntityState() const override;
    void ApplyEntityChange(const std::vector<uint8_t>& changes, size_t& pos, EChangeTypeEntity change) override;

private:
    std::array<ClientInputState, EngineDefaults::ROLLBACK_COUNT> InputStatesHistory{};
};*/
