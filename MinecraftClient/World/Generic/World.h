#pragma once
#include "Entities/Generic/CameraController.h"
#include "Entities/Generic/Entity.h"
#include "GUI/CrosshairGui.h"
#include "GUI/Gui.h"
#include "GUI/PerformanceGui.h"
#include "GUI/SelectedBlockGui.h"
#include "Util/ChunkCoords.h"
#include <algorithm>
#include <map>
#include <stack>
#include <unordered_set>

#include "Chunk.h"
#include "Entities/SP/PlayerController.h"

class World
{
protected:
    std::map<ChunkCoords, Chunk> Chunks;
    std::map<uint16_t, std::unique_ptr<Entity>> Entities;
    std::map<glm::ivec2, uint8_t, IVec2Comparator> LightLevels;
    std::unordered_set<Chunk*> DirtyChunksDuplicatorCheck{};
    std::vector<Chunk*> DirtyChunks{};
    std::vector<std::unique_ptr<Gui>> Guis;
    std::vector<Entity*> EntitiesAdded{};
    uint16_t PlayerId;
    GLuint FogsBuffer{0};
    GLFWwindow* TheAppWindow;
    const uint8_t MaxChunkRebuilt = 8;
    const uint16_t LevelHeight;
    uint64_t WorldTime{0}; //symbolises world time in ticks
    int Fps{0};
    int Frames{0};
    float LastTimeFrame{0};
    float DeltaFrameTime{0};
    float TickTimer{0};
    std::stack<uint16_t> EntityAvailableIDs{};
    std::vector<uint16_t> EntitiesToRemove{};
    const uint16_t LevelWidth;
    const uint16_t LevelDepth;
    virtual void Init();
    virtual void Tick();
    void InitFog();
    void DrawGui() const;
    void RebuildGui() const;
    void DrawWorld(float partialTick);
    World(GLFWwindow* window, uint16_t levelHeight, uint16_t levelWidth, uint16_t levelDepth);
    void UpdateChunksNear(int x, int y, int z);
    int RecalculateLightLevels(int x, int z);

public:
    virtual ~World() = default;
    World(const World&) = delete;
    World& operator=(const World&) = delete;
    World(World&&) = delete;
    World& operator=(World&&) = delete;
    [[nodiscard]] GLFWwindow* GetWindow() const;
    [[nodiscard]] int GetFps() const;
    [[nodiscard]] int GetBrightnessAt(glm::vec3 pos) const;
    [[nodiscard]] int GetBrightnessAt(int x, int y, int z) const;
    [[nodiscard]] Entity* GetEntity(uint16_t id) const;
    [[nodiscard]] PlayerController* GetPlayer() const;
    [[nodiscard]] uint64_t GetWorldTime() const;
    void HandleWindowResize(int height, int width) const;
    void AddChunkAsDirty(Chunk* chunk);
    void RemoveBlockAt(int x, int y, int z);
    bool IsBlockExists(int x, int y, int z);
    const Block* GetBlockAt(int x, int y, int z);
    EBlockType GetBlockTypeAt(int x, int y, int z);
    Chunk* GetChunkAt(int x, int y, int z);
    bool IsBlockSolid(int x, int y, int z);
    std::vector<BoundingBox> GetBlockBoxesInBoundingBox(const BoundingBox& boundingBox);
    void PlaceBlockAt(int x, int y, int z, EBlockType blockType);
    virtual void Run() = 0;
    template <typename T>
    [[nodiscard]] T* GetGuiOfType() const;
    uint16_t RegisterEntity(Entity* entity);
    virtual void RemoveEntity(uint16_t id);
    CustomRandomEngine RandomEngineState;
    void HandleKeyCallback(int key, int action) const;
    void HandleCursorPosCallback(float xPos, float yPos) const;
    void HandleMouseButtonCallback(int button, int action) const;
};

template <typename T>
T* World::GetGuiOfType() const
{
    for (auto& gui : Guis)
    {
        if (dynamic_cast<T*>(gui.get()))
        {
            return reinterpret_cast<T*>(gui.get());
        }
    }
    return nullptr;
}
