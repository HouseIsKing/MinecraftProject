#pragma once
#include <unordered_map>

#include "StateWrapper.h"
#include "Util/EngineDefaults.h"
#include "WorldState.h"

class World;

class WorldStateWrapper final : public StateWrapper<WorldState, EChangeType>
{
    std::map<ChunkCoords, Chunk> Chunks{};
    std::map<uint16_t, Player> Players{};
    std::map<uint16_t, BlockParticleEntity> BlockParticleEntities{};
    std::unordered_map<uint16_t, EEntityType> EntityIdToType{};
    std::vector<ChunkCoords> ChunksEnterWorld{};
    std::vector<ChunkCoords> ChunksLeaveWorld{};
    std::vector<uint16_t> PlayersEnterWorld{};
    std::vector<uint16_t> PlayersLeaveWorld{};
    std::vector<glm::ivec2> LightStatesChanged{};
    std::vector<glm::ivec2> LightsEnterWorld{};
    std::vector<glm::ivec2> LightsLeaveWorld{};
    std::vector<uint16_t> BlockParticleEntitiesEnterWorld{};
    std::vector<uint16_t> BlockParticleEntitiesLeaveWorld{};
    std::set<uint16_t> EntityAvailableIDs{};
    uint16_t GetValidEntityId();
    void ClearAllChanges(const EChangeType& change) override;
    void WriteChangesToVector(std::vector<uint8_t>& changes, const EChangeType& changeType) const override;

public:
    WorldStateWrapper();
    void WriteChangesToVector(std::vector<uint8_t>& changes) const override;
    void ClearAllChanges() override;
    void WriteAllDataToVector(std::vector<uint8_t>& data) const;
    void AddChunk(const ChunkState& state);
    void RemoveChunk(const ChunkCoords& coords);
    void AddEntity(EntityState* state, bool isOldState = false);
    void RemoveEntity(uint16_t entityId);
    [[nodiscard]] Chunk* GetChunk(const ChunkCoords& coords) const;
    template <class Wrapper, class StateType>
    [[nodiscard]] Entity<Wrapper, StateType>* GetEntity(const uint16_t& entityId);
    void AddLight(const glm::ivec2& coords, uint8_t value);
    void RemoveLight(const glm::ivec2& coords);
    void ChangeLight(const glm::ivec2& coords, uint8_t value);
    void SetRandomSeed(uint64_t seed);
    void SetWorldTime(uint64_t worldTime);
    [[nodiscard]] const std::unordered_map<uint16_t, EEntityType>& GetEntityIdToType() const;
    std::map<ChunkCoords, Chunk*>::iterator GetChunksIterator();
    std::map<uint16_t, Player*>::iterator GetPlayersIterator();
    std::map<uint16_t, BlockParticleEntity*>::iterator GetBlockParticleEntitiesIterator();
};

template <>
inline Entity<PlayerStateWrapper, PlayerState>* WorldStateWrapper::GetEntity<PlayerStateWrapper, PlayerState>(const uint16_t& entityId)
{
    if (State.Players.contains(entityId))
    {
        return State.Players.at(entityId);
    }
    return nullptr;
}

template <>
inline Entity<BlockParticleEntityStateWrapper, BlockParticleEntityState>* WorldStateWrapper::GetEntity<BlockParticleEntityStateWrapper, BlockParticleEntityState>(const uint16_t& entityId)
{
    if (State.BlockParticleEntities.contains(entityId))
    {
        return State.BlockParticleEntities.at(entityId);
    }
    return nullptr;
}
