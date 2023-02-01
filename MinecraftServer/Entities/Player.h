#pragma once
#include "LivingEntity.h"
#include "Network/ConnectionToClient.h"
#include "Util/EngineDefaults.h"
#include "Util/States/PlayerState.h"
#include "World/Blocks/BlockTypeList.h"

class MultiPlayerWorld;

class Player final : public LivingEntity
{
    constexpr static glm::vec3 PLAYER_SIZE = glm::vec3(0.3F, 0.9F, 0.3F);
    constexpr static float CAMERA_OFFSET = 1.62F;
    std::string Name;
    std::array<ClientInputState, EngineDefaults::ROLLBACK_COUNT> InputStates{};
    BlockFaces FaceHit;
    const Block* BlockHit;
    glm::ivec3 BlockHitPosition;
    bool Mode = false;
    EBlockType CurrentSelectedBlock;
    BlockFaces FindClosestFace(bool& foundBlock);
    [[nodiscard]] float CalculateMaxDistanceForHighlight(const glm::vec3& front, bool up, bool right, bool forward) const;
    void PlaceBlock();
    void ApplyEntityChange(const std::vector<uint8_t>& changes, size_t& pos, EChangeTypeEntity change) override;
    void HandleClientInput(const ClientInputState& inputState) const;

public:
    Player(float x, float y, float z);
    explicit Player(const PlayerState& state);
    void Tick() override;
    [[nodiscard]] bool GetMode() const;
    [[nodiscard]] EBlockType GetCurrentSelectedBlock() const;
    [[nodiscard]] EntityState* GetEntityState() const override;
    void SetClientInputOnTick(uint64_t tick, const ClientInputState& inputState);
};
