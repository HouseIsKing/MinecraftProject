#pragma once
#include "Chunk.h"
#include "Entities/Player.h"
#include <stack>

#include "Network/ConnectionToClientInterface.h"
#include "Network/ServerNetworkManager.h"
#include "Util/States/WorldState.h"

class MultiPlayerWorld
{
public:
    MultiPlayerWorld(uint16_t width, uint16_t height, uint16_t depth);
    ~MultiPlayerWorld();
    MultiPlayerWorld(const MultiPlayerWorld& other) = delete;
    MultiPlayerWorld& operator=(const MultiPlayerWorld& other) = delete;
    MultiPlayerWorld(MultiPlayerWorld&& other) = delete;
    MultiPlayerWorld& operator=(MultiPlayerWorld&& other) = delete;
    uint16_t RegisterEntity(Entity* entity);
    void RemoveEntity(uint16_t id);
    void Tick();
    void Run();
    bool IsBlockExists(int x, int y, int z);
    void PlaceBlockAt(int x, int y, int z, EBlockType blockType);
    void RemoveBlockAt(int x, int y, int z);
    [[nodiscard]] Entity* GetEntity(uint16_t id) const;
    const Block* GetBlockAt(int x, int y, int z);
    EBlockType GetBlockTypeAt(int x, int y, int z);
    Chunk* GetChunkAt(int x, int y, int z);
    [[nodiscard]] int GetBrightnessAt(int x, int y, int z) const;
    bool IsBlockSolid(int x, int y, int z);
    [[nodiscard]] int GetBrightnessAt(glm::vec3 pos) const;
    [[nodiscard]] uint64_t GetWorldTime() const;
    [[nodiscard]] uint64_t GetMaxWorldTime() const;
    void HandlePacket(const PacketData* packet);
    std::vector<BoundingBox> GetBlockBoxesInBoundingBox(const BoundingBox& boundingBox);
    CustomRandomEngine RandomEngineState;

private:
    std::map<ChunkCoords, Chunk> Chunks;
    std::map<uint16_t, std::unique_ptr<Entity>> Entities;
    std::map<glm::ivec2, uint8_t, IVec2Comparator> LightLevels;
    uint64_t WorldTime = 0;
    uint64_t MaxWorldTime = 0;
    std::array<WorldState, EngineDefaults::ROLLBACK_COUNT> WorldStates{};
    ServerNetworkManager NetworkManager;
    std::unordered_map<std::shared_ptr<ConnectionToClientInterface>, Player*, ConnectionHasher, ConnectionEqual> Connections;
    std::stack<uint16_t> EntityAvailableIDs{};
    std::vector<uint16_t> EntitiesToRemove{};
    std::vector<Entity*> EntitiesAdded{};
    const uint16_t LevelWidth;
    const uint16_t LevelHeight;
    const uint16_t LevelDepth;
    void SaveWorld();
    void LoadWorld();
    void GenerateChunks(uint16_t amountX, uint16_t amountY, uint16_t amountZ);
    void GenerateLevel();
    void Init();
    void RecalculateLightLevels();
    int RecalculateLightLevels(int x, int z);
    void BuildWorldState();
    bool RevertWorldState(uint64_t tick);
    void SimulateTicks(uint8_t tickCount);
    void ApplyChangesList(const std::vector<uint8_t>& changes);
};
