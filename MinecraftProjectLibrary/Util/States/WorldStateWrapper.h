#pragma once
#include "StateWrapper.h"
#include "Util/EngineDefaults.h"
#include "WorldState.h"

class World;

class WorldStateWrapper final : public StateWrapper<WorldState, EChangeType>
{
    std::vector<ChunkCoords> ChunksEnterWorld;
    std::vector<ChunkCoords> ChunksLeaveWorld;
    std::vector<uint16_t> PlayersEnterWorld;
    std::vector<uint16_t> PlayersLeaveWorld;
    std::vector<glm::ivec2> LightStatesChanged;
    std::vector<glm::ivec2> LightsEnterWorld;
    std::vector<glm::ivec2> LightsLeaveWorld;

public:
    WorldStateWrapper() = default;
    void WriteChangesToVector(std::vector<uint8_t>& changes) const override;
    void ClearAllChanges() override;
    void WriteAllDataToVector(std::vector<uint8_t>& data) const;
    void AddChunk(int x, int y, int z);
    void AddChunk(const ChunkState& state);
    void RemoveChunk(const ChunkCoords& coords);
    void AddPlayer(const uint16_t& playerId, float x, float y, float z);
    void AddPlayer(const PlayerState& state);
    void RemovePlayer(const uint16_t& playerId);
    void AddLight(const glm::ivec2& coords, uint8_t value);
    void RemoveLight(const glm::ivec2& coords);
    void ChangeLight(const glm::ivec2& coords, uint8_t value);
    void SetRandomSeed(uint64_t seed);
    void SetWorldTime(uint64_t worldTime);
    Chunk& GetChunk(const ChunkCoords& coords);
    Player& GetPlayer(const uint16_t& playerId);
    std::map<ChunkCoords, Chunk>::iterator GetChunksIterator();
    std::map<uint16_t, Player>::iterator GetPlayersIterator();
};
