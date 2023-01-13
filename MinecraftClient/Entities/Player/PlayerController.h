#pragma once
#include "../LivingEntity.h"
#include "Camera.h"
#include "GUI/SelectedBlockGui.h"
#include "PlayerSelectionHighlight.h"

class CameraController
{
    static std::unique_ptr<Camera> ActiveCamera;

public:
    static void SetActiveCamera(Camera& camera);
    static Camera& GetActiveCamera();
    static void OnResizeWindow(int width, int height);
};

class PlayerController final : public LivingEntity
{
    Camera& MyCamera;
    constexpr static glm::vec3 PLAYER_SIZE = glm::vec3(0.3F, 0.9F, 0.3F);
    constexpr static float CAMERA_OFFSET = 1.62F;
    bool LeftMousePressed;
    bool RightMousePressed;
    float PrevMouseX;
    float PrevMouseY;
    float MouseSensitivity = 0.15F;
    bool IsSpawnZombieButtonPressed;
    bool Mode = false;
    EBlockType CurrentSelectedBlock;
    SelectedBlockGui* SelectedBlockGuiPtr;
    PlayerSelectionHighlight SelectionHighlight;
    static int GetSelectionHighlightBrightness(int x, int y, int z, BlockFaces face);
    BlockFaces FindClosestFace(glm::ivec3& blockPosition, bool& foundBlock) const;
    [[nodiscard]] float CalculateMaxDistanceForHighlight(const glm::vec3& front, bool up, bool right, bool forward) const;
    void PlaceBlock() const;
    void HandleMouseInput();
    void HandleKeyboardMovementInput();

public:
    void DisplaySelectionHighlight();
    PlayerController(float x, float y, float z);
    void Render(float partialTick) override;
    [[nodiscard]] Frustum GetCameraFrustum() const;
    [[nodiscard]] float GetCameraPitch() const;
    [[nodiscard]] bool GetMode() const;
    [[nodiscard]] EBlockType GetCurrentSelectedBlock() const;
};
