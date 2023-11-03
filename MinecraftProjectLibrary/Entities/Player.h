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
    std::queue<std::pair<uint64_t, ClientInputStatusStruct>> ClientInputQueue{};
    std::uint64_t LastInputProcessed{0};
    BlockFaces FindClosestFace(bool& foundBlock, glm::ivec3& hitPosition) const;
    [[nodiscard]] static float CalculateMaxDistanceForHighlight(const glm::vec3& front, bool up, bool right, bool forward);
    void PlaceBlock(const glm::ivec3& hitPos, const BlockFaces& faceHit) const;
    void ApplyRevertEntityChange(const std::vector<uint8_t>& changes, size_t& pos, EChangeTypeEntity change, bool revert = true) override;
    void HandleClientInput();

public:
    Player(float x, float y, float z, const uint16_t& id);
    explicit Player(const PlayerState& state);
    void Tick() override;
    void AddClientInputToQueue(const ClientInputStatusStruct& input, const uint64_t& inputId);
    [[nodiscard]] uint64_t GetLastInputProcessed() const;
};
