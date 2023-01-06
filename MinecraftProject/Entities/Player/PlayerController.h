#pragma once
#include "../LivingEntity.h"
#include "Camera.h"
#include <memory>

#include "GUI/SelectedBlockGui.h"
#include "PlayerSelectionHighlight.h"

using std::unique_ptr;

class CameraController
{
    static unique_ptr<Camera> ActiveCamera;
public:
    static void SetActiveCamera(Camera& camera);
    static Camera& GetActiveCamera();
    static void OnResizeWindow(int width, int height);
};

class PlayerController final : public LivingEntity
{
    Camera& MyCamera;
    constexpr static vec3 PLAYER_SIZE = vec3(0.3F, 0.9F, 0.3F);
    constexpr static float CAMERA_OFFSET = 1.62F;
    bool LeftMousePressed;
    bool RightMousePressed;
    float PrevMouseX;
    float PrevMouseY;
    float MouseSensitivity = 0.15F;
    bool IsSpawnZombieButtonPressed;
    EBlockType CurrentSelectedBlock;
    SelectedBlockGui* SelectedBlockGuiPtr;
    PlayerSelectionHighlight SelectionHighlight;
    static int GetSelectionHighlightBrightness(int x, int y, int z, BlockFaces face);
    BlockFaces FindClosestFace(glm::ivec3& blockPosition, bool& foundBlock) const;
    [[nodiscard]] float CalculateMaxDistanceForHighlight(const vec3& front, bool up, bool right, bool forward) const;
    void PlaceBlock() const;
    void HandleMouseInput();
    void HandleKeyboardMovementInput();

public:
    void DisplaySelectionHighlight();
    PlayerController(float x, float y, float z);
    void Render(float partialTick) override;
    [[nodiscard]] Frustum GetCameraFrustum() const;
};
