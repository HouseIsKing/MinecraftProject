#pragma once
#include "Entities/Generic/CameraController.h"
#include "Entities/Generic/LivingEntity.h"
#include "Entities/Generic/PlayerSelectionHighlight.h"
#include "Entities/Generic/Zombie.h"
#include "GUI/SelectedBlockGui.h"
#include "Util/States/ClientInputState.h"
#include "Util/States/PlayerState.h"

class PlayerController : public LivingEntity
{
    Camera& MyCamera;
    constexpr static glm::vec3 PLAYER_SIZE = glm::vec3(0.3F, 0.9F, 0.3F);
    constexpr static float CAMERA_OFFSET = 1.62F;
    float PrevMouseX;
    float PrevMouseY;
    float PrevPitch;
    float MouseSensitivity = 0.15F;
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

protected:
    ClientInputState InputState{};

public:
    void DisplaySelectionHighlight();
    PlayerController(float x, float y, float z);
    explicit PlayerController(const PlayerState& playerState);
    void Render(float partialTick) override;
    void Tick() override;
    [[nodiscard]] Frustum GetCameraFrustum() const;
    [[nodiscard]] float GetCameraPitch() const;
    [[nodiscard]] bool GetMode() const;
    [[nodiscard]] EBlockType GetCurrentSelectedBlock() const;
    void UpdateMouseMove(float x, float y);
    void KeyboardButtonPressed(int button, int action);
    void MouseButtonPressed(int button, int action);
};