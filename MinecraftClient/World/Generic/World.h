#pragma once
#include "Entities/Generic/CameraController.h"
#include "Entities/Generic/Entity.h"
#include "GUI/CrosshairGui.h"
#include "GUI/Gui.h"
#include "GUI/PerformanceGui.h"
#include "GUI/SelectedBlockGui.h"
#include "Util/ChunkCoords.h"
#include <algorithm>
#include <stack>
#include <unordered_set>

struct IntegerVector2Hasher
{
    size_t operator()(const glm::ivec2& vector) const
    {
        const size_t x = vector.x >= 0 ? static_cast<unsigned long long>(2 * vector.x) : static_cast<unsigned long long>(-2 * vector.x - 1);
        const size_t y = vector.y >= 0 ? static_cast<unsigned long long>(2 * vector.y) : static_cast<unsigned long long>(-2 * vector.y - 1);
        return x >= y ? x * x + x + y : y * y + x;
    }
};

template <typename WorldType, typename PlayerType>
class World
{
protected:
    std::unordered_map<ChunkCoords<WorldType>, Chunk<WorldType>, ChunkComparator<WorldType>> Chunks;
    std::unordered_map<uint16_t, std::unique_ptr<Entity<WorldType>>> Entities;
    std::unordered_set<Chunk<WorldType>*> DirtyChunksDuplicatorCheck{};
    std::vector<Chunk<WorldType>*> DirtyChunks{};
    std::vector<std::unique_ptr<Gui<WorldType>>> Guis;
    std::unordered_map<glm::ivec2, uint8_t, IntegerVector2Hasher> LightLevels;
    PlayerType* Player;
    GLuint FogsBuffer{0};
    GLFWwindow* TheAppWindow;
    const uint8_t MaxChunkRebuilt = 8;
    long WorldTime{0}; //symbolises world time in ticks
    int Fps{0};
    int Frames{0};
    float LastTimeFrame{0};
    float DeltaFrameTime{0};
    float TickTimer{0};
    virtual void Init();
    void InitFog();
    void DrawGui() const;
    void RebuildGui() const;
    void DrawWorld(float partialTick);
    World(GLFWwindow* window, WorldType* world);

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
    [[nodiscard]] Entity<WorldType>* GetEntity(uint16_t id) const;
    [[nodiscard]] PlayerType* GetPlayer() const;
    void HandleWindowResize(int height, int width) const;
    void AddChunkAsDirty(Chunk<WorldType>* chunk);
    bool IsBlockExists(int x, int y, int z);
    const Block* GetBlockAt(int x, int y, int z);
    EBlockType GetBlockTypeAt(int x, int y, int z);
    Chunk<WorldType>* GetChunkAt(int x, int y, int z);
    bool IsBlockSolid(int x, int y, int z);
    std::vector<BoundingBox> GetBlockBoxesInBoundingBox(const BoundingBox& boundingBox);
    virtual void Run() = 0;
    template <typename T>
    [[nodiscard]] Gui<WorldType>* GetGuiOfType() const;
    virtual void RemoveEntity(uint16_t id);
    static constexpr float TICK_RATE = 0.05F;
};

template <typename WorldType, typename PlayerType>
template <typename T>
Gui<WorldType>* World<WorldType, PlayerType>::GetGuiOfType() const
{
    for (auto& gui : Guis)
    {
        if (dynamic_cast<T*>(gui.get()))
        {
            return gui.get();
        }
    }
    return nullptr;
}

template <typename WorldType, typename PlayerType>
void World<WorldType, PlayerType>::RemoveEntity(uint16_t id)
{
}

template <typename WorldType, typename PlayerType>
void World<WorldType, PlayerType>::Init()
{
    Shader::SetUnsignedInt(EngineDefaults::GetShader()->GetUniformInt("worldTime"), static_cast<GLuint>(WorldTime % 24000L));
    InitFog();
    EngineDefaults::InitTextures();
}

template <typename WorldType, typename PlayerType>
void World<WorldType, PlayerType>::InitFog()
{
    const std::array fogs{14.0F / 255.0F, 11.0F / 255.0F, 10.0F / 255.0F, 1.0F, 0.01F, 0.0F, 0.0F, 0.0F, 254.0F / 255.0F, 251.0F / 255.0F, 250.0F / 255.0F, 1.0F, 0.001F, 0.0F, 0.0F, 0.0F};
    glGenBuffers(1, &FogsBuffer);
    glBindBuffer(GL_UNIFORM_BUFFER, FogsBuffer);
    glBufferData(GL_UNIFORM_BUFFER, static_cast<GLintptr>(fogs.size() * sizeof(float)), fogs.data(), GL_STATIC_COPY);
    glBindBufferRange(GL_UNIFORM_BUFFER, 1, FogsBuffer, 0, static_cast<GLintptr>(fogs.size() * sizeof(float)));
    glUniformBlockBinding(EngineDefaults::GetShader()->Program, EngineDefaults::GetShader()->GetUniformBlockIndex("fogBlock"), 1);
}

template <typename WorldType, typename PlayerType>
void World<WorldType, PlayerType>::DrawGui() const
{
    Shader::SetUnsignedInt(EngineDefaults::GetShader(1)->GetUniformInt("worldTime"), static_cast<GLuint>(WorldTime % 24000L));
    for (const auto& gui : Guis)
    {
        gui->Render();
    }
}

template <typename WorldType, typename PlayerType>
void World<WorldType, PlayerType>::RebuildGui() const
{
    for (const auto& gui : Guis)
    {
        gui->Rebuild();
    }
}

template <typename WorldType, typename PlayerType>
void World<WorldType, PlayerType>::DrawWorld(float partialTick)
{
    glEnable(GL_CULL_FACE);
    Player->Render(partialTick);
    const Frustum frustum = Player->GetCameraFrustum();
    for (const auto& entity : Entities | std::views::values)
    {
        if (entity.get() != dynamic_cast<Entity<WorldType>*>(Player) && frustum.CubeInFrustum(entity->GetBoundingBox()))
        {
            entity->Render(partialTick);
        }
    }
    std::ranges::sort(DirtyChunks, DirtyChunkComparator<WorldType>{frustum});
    for (int i = 0; i < MaxChunkRebuilt && !DirtyChunks.empty(); i++)
    {
        DirtyChunks[0]->GenerateTessellationData();
        DirtyChunksDuplicatorCheck.erase(DirtyChunks[0]);
        DirtyChunks.erase(DirtyChunks.begin());
    }
    for (auto& chunk : Chunks | std::views::values)
    {
        if (chunk.ChunkInFrustum(frustum))
        {
            chunk.Draw();
        }
    }
    Player->DisplaySelectionHighlight();
    DrawGui();
    glDisable(GL_CULL_FACE);
    Frames++;
    const auto time = static_cast<float>(glfwGetTime());
    DeltaFrameTime += time - LastTimeFrame;
    TickTimer += time - LastTimeFrame;
    LastTimeFrame = time;
    if (DeltaFrameTime > 1.0F)
    {
        DeltaFrameTime -= 1.0F;
        Fps = Frames;
        Frames = 0;
    }
}

template <typename WorldType, typename PlayerType>
World<WorldType, PlayerType>::World(GLFWwindow* window, WorldType* world) : Player(nullptr), TheAppWindow(window)
{
    Entity<WorldType>::SetWorld(world);
    Chunk<WorldType>::SetWorld(world);
    Gui<WorldType>::SetWorld(world);
    Guis.emplace_back(new CrosshairGui<WorldType>());
    Guis[0]->Active = true;
    Guis.emplace_back(new SelectedBlockGui<WorldType>());
    Guis[1]->Active = true;
    Guis.emplace_back(new PerformanceGui<WorldType>());
    Guis[2]->Active = true;
}

template <typename WorldType, typename PlayerType>
GLFWwindow* World<WorldType, PlayerType>::GetWindow() const
{
    return TheAppWindow;
}

template <typename WorldType, typename PlayerType>
int World<WorldType, PlayerType>::GetFps() const
{
    return Fps;
}

template <typename WorldType, typename PlayerType>
int World<WorldType, PlayerType>::GetBrightnessAt(const glm::vec3 pos) const
{
    return GetBrightnessAt(static_cast<int>(pos.x), static_cast<int>(pos.y), static_cast<int>(pos.z));
}

template <typename WorldType, typename PlayerType>
int World<WorldType, PlayerType>::GetBrightnessAt(const int x, const int y, const int z) const
{
    if (const auto pos = glm::ivec2(x, z); LightLevels.contains(pos))
    {
        if (const uint8_t lightLevel = LightLevels.at(glm::ivec2(x, z)); y > lightLevel)
        {
            return 1;
        }
    }
    return 0;
}

template <typename WorldType, typename PlayerType>
Entity<WorldType>* World<WorldType, PlayerType>::GetEntity(uint16_t id) const
{
    if (Entities.contains(id))
    {
        return Entities.at(id).get();
    }
    return nullptr;
}

template <typename WorldType, typename PlayerType>
PlayerType* World<WorldType, PlayerType>::GetPlayer() const
{
    return Player;
}

template <typename WorldType, typename PlayerType>
void World<WorldType, PlayerType>::HandleWindowResize(const int height, const int width) const
{
    glViewport(0, 0, width, height);
    CameraController::OnResizeWindow(width, height);
    RebuildGui();
}

template <typename WorldType, typename PlayerType>
void World<WorldType, PlayerType>::AddChunkAsDirty(Chunk<WorldType>* chunk)
{
    if (DirtyChunksDuplicatorCheck.insert(chunk).second)
    {
        chunk->GotDirty();
        DirtyChunks.push_back(chunk);
    }
}

template <typename WorldType, typename PlayerType>
bool World<WorldType, PlayerType>::IsBlockExists(const int x, const int y, const int z)
{
    return GetBlockTypeAt(x, y, z) != EBlockType::Air;
}

template <typename WorldType, typename PlayerType>
const Block* World<WorldType, PlayerType>::GetBlockAt(const int x, const int y, const int z)
{
    return BlockTypeList::GetBlockTypeData(GetBlockTypeAt(x, y, z));
}

template <typename WorldType, typename PlayerType>
EBlockType World<WorldType, PlayerType>::GetBlockTypeAt(int x, int y, int z)
{
    const Chunk<WorldType>* chunk = GetChunkAt(x, y, z);
    if (chunk == nullptr)
    {
        return EBlockType::Air;
    }
    return chunk->GetBlockTypeAt(x, y, z);
}

template <typename WorldType, typename PlayerType>
Chunk<WorldType>* World<WorldType, PlayerType>::GetChunkAt(int x, int y, int z)
{
    if (const auto pos = ChunkCoords<WorldType>(x, y, z); Chunks.contains(pos))
    {
        return &Chunks.at(pos);
    }
    return nullptr;
}

template <typename WorldType, typename PlayerType>
bool World<WorldType, PlayerType>::IsBlockSolid(int x, int y, int z)
{
    return GetBlockAt(x, y, z)->IsSolidBlock();
}

template <typename WorldType, typename PlayerType>
std::vector<BoundingBox> World<WorldType, PlayerType>::GetBlockBoxesInBoundingBox(const BoundingBox& boundingBox)
{
    std::vector<BoundingBox> result{};
    for (int x = static_cast<int>(boundingBox.GetMinX()); static_cast<float>(x) <= boundingBox.GetMaxX(); x++)
    {
        for (int y = static_cast<int>(boundingBox.GetMinY()); static_cast<float>(y) <= boundingBox.GetMaxY(); y++)
        {
            for (int z = static_cast<int>(boundingBox.GetMinZ()); static_cast<float>(z) <= boundingBox.GetMaxZ(); z++)
            {
                if (const Block* block = GetBlockAt(x, y, z); block->IsSolidBlock())
                {
                    BoundingBox helper = block->GetBoundingBox();
                    helper.Move(static_cast<float>(x), static_cast<float>(y), static_cast<float>(z));
                    result.push_back(helper);
                }
            }
        }
    }
    return result;
}
