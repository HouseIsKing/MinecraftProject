#pragma once
#include "LivingEntity.h"
#include "Util/States/PlayerStateWrapper.h"
#include "World/Blocks/Block.h"
#include <array>

class World;

class Player final : public LivingEntity<PlayerStateWrapper, PlayerState>
{
    //std::string Name;
    std::array<ClientInputStruct, EngineDefaults::ROLLBACK_COUNT> ClientInputs{};
    BlockFaces FaceHit;
    const Block* BlockHit;
    glm::ivec3 BlockHitPosition;
    bool Mode = false;
    EBlockType CurrentSelectedBlock;
    BlockFaces FindClosestFace(bool& foundBlock);
    [[nodiscard]] float CalculateMaxDistanceForHighlight(const glm::vec3& front, bool up, bool right, bool forward) const;
    void PlaceBlock() const;
    void ApplyRevertEntityChange(const std::vector<uint8_t>& changes, size_t& pos, EChangeTypeEntity change, bool revert = true) override;
    void HandleClientInput();

public:
    Player(float x, float y, float z, const uint16_t& id);
    explicit Player(const PlayerState& state);
    void Tick() override;
    void NewTick();
    [[nodiscard]] bool GetMode() const;
    [[nodiscard]] EBlockType GetCurrentSelectedBlock() const;
    void SetClientInput(uint64_t index, const ClientInputStruct& input);
};
