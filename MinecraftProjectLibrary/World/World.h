#pragma once
#include "Util/States/WorldStateWrapper.h"
#include <memory>
#include <stack>

class PacketData;

class World
{
public:
    static World* GetWorld();
    virtual ~World();
    World(const World& other) = delete;
    World& operator=(const World& other) = delete;
    World(World&& other) = delete;
    World& operator=(World&& other) = delete;
    virtual void Run() = 0;
    virtual void NewTick() = 0;
    virtual void PreTick();
    virtual void HandlePacket(const PacketData* packet) = 0;
    virtual void EntityAdded(uint16_t entityId);
    virtual void EntityRemoved(uint16_t entityId);
    void BlockChanged(int x, int y, int z, EBlockType newBlock, EBlockType oldBlock);
    void LightChanged(int x, int y, int z, int16_t change);
    [[nodiscard]] bool IsBlockExists(int x, int y, int z) const;
    void PlaceBlockAt(int x, int y, int z, EBlockType blockType);
    void RemoveBlockAt(int x, int y, int z);
    [[nodiscard]] const Block* GetBlockAt(int x, int y, int z) const;
    [[nodiscard]] EBlockType GetBlockTypeAt(int x, int y, int z) const;
    [[nodiscard]] Chunk* GetChunkAt(int x, int y, int z) const;
    [[nodiscard]] int GetBrightnessAt(int x, int y, int z) const;
    [[nodiscard]] bool IsBlockSolid(int x, int y, int z) const;
    [[nodiscard]] int GetBrightnessAt(glm::vec3 pos) const;
    [[nodiscard]] uint64_t GetWorldTime() const;
    [[nodiscard]] std::vector<BoundingBox> GetBlockBoxesInBoundingBox(const BoundingBox& boundingBox) const;
    template <typename StateWrapper, typename StateType>
    [[nodiscard]] std::pair<const StateType&, const StateType&> GetEntityState(uint16_t entityId);
    CustomRandomEngine TickRandomEngine;
    std::vector<std::unique_ptr<EntityState>> EntitiesToAdd{};
    std::vector<uint16_t> EntitiesToRemove{};

protected:
    World(uint16_t width, uint16_t height, uint16_t depth);
    static World* TheWorld;
    WorldStateWrapper State;
    Player PlayerTrash{PlayerState{}};
    BlockParticleEntity BlockParticleTrash{BlockParticleEntityState{}};
    std::array<std::vector<uint8_t>, EngineDefaults::ROLLBACK_COUNT> ChangesLists{};
    uint16_t LevelWidth;
    uint16_t LevelHeight;
    uint16_t LevelDepth;
    std::uint64_t InputId{0};
    void SaveWorld() const;
    void LoadWorld();
    void GenerateChunks(uint16_t amountX, uint16_t amountY, uint16_t amountZ);
    void GenerateLevel();
    void Tick();
    void RecalculateLightLevels();
    void RecalculateLightLevels(int x, int z);
    void BuildWorldChanges();
    bool RevertWorldState(uint64_t tick);
    void SimulateTicks(const std::map<uint64_t, ClientInputStatusStruct>& inputYetToBeProcessed, uint16_t playerEntityId);
    void RevertChangesList(const std::vector<uint8_t>& changes);
    void UpdateChunksNear(int x, int y, int z);
    void ProcessAddingEntities();
    void ProcessRemovingEntities();
    void ProcessTickEntities();
    virtual void ChunkChanged(const ChunkCoords& coords);
    virtual void ChunkAdded(const ChunkCoords& coords);
    virtual void ChunkRemoved(const ChunkCoords& coords);
};

/*
 * returns pair of states, first is current state, second is old state
 */
template <typename StateWrapper, typename StateType>
std::pair<const StateType&, const StateType&> World::GetEntityState(const uint16_t entityId)
{
    Entity<StateWrapper, StateType>* entity = State.GetEntity<StateWrapper, StateType>(entityId);
    return {entity->GetState(), entity->GetOldState()};
}
