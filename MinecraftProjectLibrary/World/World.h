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
    virtual void HandlePacket(const PacketData* packet) = 0;
    bool IsBlockExists(int x, int y, int z);
    void PlaceBlockAt(int x, int y, int z, EBlockType blockType);
    void RemoveBlockAt(int x, int y, int z);
    void RemovePlayer(uint16_t id);
    Player& GetPlayer(uint16_t id);
    const Block* GetBlockAt(int x, int y, int z);
    EBlockType GetBlockTypeAt(int x, int y, int z);
    Chunk* GetChunkAt(int x, int y, int z);
    [[nodiscard]] int GetBrightnessAt(int x, int y, int z) const;
    bool IsBlockSolid(int x, int y, int z);
    [[nodiscard]] int GetBrightnessAt(glm::vec3 pos) const;
    [[nodiscard]] uint64_t GetWorldTime() const;
    std::vector<BoundingBox> GetBlockBoxesInBoundingBox(const BoundingBox& boundingBox);
    CustomRandomEngine TickRandomEngine;

protected:
    World(uint16_t width, uint16_t height, uint16_t depth);
    static World* TheWorld;
    WorldStateWrapper State;
    std::array<std::vector<uint8_t>, EngineDefaults::ROLLBACK_COUNT> ChangesLists{};
    std::stack<uint16_t> EntityAvailableIDs{};
    std::vector<uint16_t> PlayersToRemove{};
    std::vector<PlayerState> PlayersToAdd{};
    const uint16_t LevelWidth;
    const uint16_t LevelHeight;
    const uint16_t LevelDepth;
    void SaveWorld() const;
    void LoadWorld();
    void GenerateChunks(uint16_t amountX, uint16_t amountY, uint16_t amountZ);
    void GenerateLevel();
    void Tick();
    void RecalculateLightLevels();
    int16_t RecalculateLightLevels(int x, int z);
    void BuildWorldChanges();
    bool RevertWorldState(uint64_t tick);
    void SimulateTicks(uint8_t tickCount);
    void RevertChangesList(const std::vector<uint8_t>& changes);
};
