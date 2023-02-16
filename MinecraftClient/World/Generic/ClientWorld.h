#pragma once
#include "ChunkRenderer.h"
#include "Entities/ClientPlayerRenderer.h"
#include "Entities/Generic/BlockParticleEntityRenderer.h"
#include "Entities/PlayerInputManager.h"
#include "GUI/Gui.h"
#include "World/World.h"
#include <GLFW/glfw3.h>
#include <unordered_set>

class ClientWorld : public World
{
    static ClientWorld* TheWorld;

protected:
    std::unordered_map<ChunkCoords, ChunkRenderer, ChunkCoordsHasher> ChunkRenderers{};
    std::unordered_map<uint16_t, PlayerRenderer> PlayerRenderers{};
    std::unordered_map<uint16_t, BlockParticleEntityRenderer> BlockParticleEntityRenderers{};
    std::unordered_set<ChunkRenderer*> DirtyChunksDuplicatorCheck{};
    std::vector<ChunkRenderer*> DirtyChunks{};
    std::vector<std::unique_ptr<Gui>> Guis{};
    PlayerInputManager PlayerInput{};
    std::unique_ptr<ClientPlayerRenderer> LocalPlayerRenderer{};
    uint16_t PlayerId;
    GLuint FogsBuffer{0};
    GLFWwindow* TheAppWindow;
    const uint8_t MaxChunkRebuilt = 8;
    int Fps{0};
    int Frames{0};
    float LastTimeFrame{0};
    float DeltaFrameTime{0};
    float TickTimer{0};
    void Init();
    void InitFog();
    void DrawGui() const;
    void RebuildGui() const;
    void DrawWorld(float partialTick);
    void ChunkChanged(const ChunkCoords& coords) override;
    void ChunkAdded(const ChunkCoords& coords) override;
    void ChunkRemoved(const ChunkCoords& coords) override;
    void EntityAdded(uint16_t entityId) override;
    void EntityChanged(uint16_t entityId) override;
    void EntityRemoved(uint16_t entityId) override;
    ClientWorld(GLFWwindow* window, uint16_t levelHeight, uint16_t levelWidth, uint16_t levelDepth);

public:
    CustomRandomEngine RenderRandomEngine{};
    [[nodiscard]] GLFWwindow* GetWindow() const;
    [[nodiscard]] int GetFps() const;
    [[nodiscard]] const Player* GetPlayer() const;
    void HandleWindowResize(int height, int width) const;
    void NewTick() override;
    void Run() override;
    template <typename T>
    [[nodiscard]] T* GetGuiOfType() const;
    void AddGui(Gui* gui);
    void HandleKeyCallback(int key, int action);
    void HandleCursorPosCallback(float xPos, float yPos);
    void HandleMouseButtonCallback(int button, int action);
    static ClientWorld* GetWorld();
};

template <typename T>
T* ClientWorld::GetGuiOfType() const
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
