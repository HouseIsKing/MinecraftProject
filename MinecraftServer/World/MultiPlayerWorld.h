#pragma once
#include "Chunk.h"
#include "Entities/Player.h"
#include <stack>

#include "Network/ConnectionToClientInterface.h"
#include "Network/ServerNetworkManager.h"

struct IntegerVector2Hasher
{
    size_t operator()(const glm::ivec2& vector) const
    {
        const size_t x = vector.x >= 0 ? static_cast<unsigned long long>(2 * vector.x) : static_cast<unsigned long long>(-2 * vector.x - 1);
        const size_t y = vector.y >= 0 ? static_cast<unsigned long long>(2 * vector.y) : static_cast<unsigned long long>(-2 * vector.y - 1);
        return x >= y ? x * x + x + y : y * y + x;
    }
};

class MultiPlayerWorld
{
    std::unordered_map<ChunkCoords, Chunk, ChunkComparator> Chunks;
    std::unordered_map<uint16_t, std::unique_ptr<Entity>> Entities;
    ServerNetworkManager NetworkManager;
    std::unordered_map<std::shared_ptr<ConnectionToClientInterface>, Player*, ConnectionHasher, ConnectionEqual> Connections;
    std::stack<uint16_t> EntityAvailableIDs{};
    std::vector<uint16_t> EntitiesToRemove{};
    long WorldTime; //symbolises world time in ticks
    const uint16_t LevelWidth;
    const uint16_t LevelHeight;
    const uint16_t LevelDepth;
    std::unordered_map<glm::ivec2, uint8_t, IntegerVector2Hasher> LightLevels;
    void SaveWorld();
    void LoadWorld();
    void GenerateChunks(uint16_t amountX, uint16_t amountY, uint16_t amountZ);
    void GenerateLevel();
    void Init();
    void RecalculateLightLevels();
    int RecalculateLightLevels(int x, int z);
    void SendEntireWorldToClient(ConnectionToClient* client) const;

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
    std::vector<std::shared_ptr<Packet>> GetTickPackets() const;
    std::vector<BoundingBox> GetBlockBoxesInBoundingBox(const BoundingBox& boundingBox);
};
