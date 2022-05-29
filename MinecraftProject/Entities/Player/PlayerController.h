//
// Created by amit on 4/22/2022.
//

#pragma once
#include "../LivingEntity.h"
#include "Camera.h"
#include <memory>

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
    PlayerSelectionHighlight SelectionHighlight;
    static int GetSelectionHighlightBrightness(int x, int y, int z, BlockFaces face);
    void DisplaySelectionHighlight();
    BlockFaces FindClosestFace(glm::ivec3& blockPosition, bool& foundBlock) const;
    [[nodiscard]] float CalculateMaxDistanceForHighlight(const vec3& front, bool up, bool right, bool forward) const;
    void PlaceBlock() const;
public:
    PlayerController(uint16_t entityId, float x, float y, float z);
    void HandleMouseInput();
    void HandleKeyboardMovementInput();
    void Tick() override;
    void Render() override;
    [[nodiscard]] Frustum GetCameraFrustum() const;
};
