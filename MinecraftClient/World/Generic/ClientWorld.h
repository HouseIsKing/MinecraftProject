#pragma once
#include "Entities/Generic/CameraController.h"
#include "GUI/Gui.h"
#include <algorithm>
#include <map>
#include <stack>
#include <unordered_set>
#include <GLFW/glfw3.h>

#include "ChunkRenderer.h"
#include "Entities/ClientPlayerRenderer.h"
#include "Entities/PlayerInputManager.h"
#include "Entities/SP/PlayerRenderer.h"
#include "Util/CustomRandomEngine.h"
#include "World/World.h"

class ClientWorld : public World
{
    static ClientWorld* TheWorld;

protected:
    std::unordered_map<ChunkCoords, ChunkRenderer, ChunkCoordsHasher> ChunkRenderers{};
    std::unordered_map<uint16_t, PlayerRenderer> PlayerRenderers{};
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
    ClientWorld(GLFWwindow* window, uint16_t levelHeight, uint16_t levelWidth, uint16_t levelDepth);
    void UpdateChunksNear(int x, int y, int z);

public:
    [[nodiscard]] GLFWwindow* GetWindow() const;
    [[nodiscard]] int GetFps() const;
    [[nodiscard]] const Player& GetPlayer() const;
    void HandleWindowResize(int height, int width) const;
    void AddChunkAsDirty(ChunkRenderer* chunk);
    void NewTick() override;
    template <typename T>
    [[nodiscard]] T* GetGuiOfType() const;
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
