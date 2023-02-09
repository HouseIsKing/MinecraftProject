#pragma once
#include "Entities/Generic/LivingEntityRenderer.h"
#include "Util/States/PlayerState.h"


class PlayerRenderer : public LivingEntityRenderer<PlayerState>
{
    /*
    Camera& MyCamera;
    float MouseSensitivity = 0.15F;
    */
    //bool Mode = false;
    //EBlockType CurrentSelectedBlock;
    //SelectedBlockGui* SelectedBlockGuiPtr;
    //PlayerSelectionHighlight SelectionHighlight;
    //static int GetSelectionHighlightBrightness(int x, int y, int z, BlockFaces face);
    //BlockFaces FindClosestFace(glm::ivec3& blockPosition, bool& foundBlock) const;
    //[[nodiscard]] float CalculateMaxDistanceForHighlight(const glm::vec3& front, bool up, bool right, bool forward) const;
    //void PlaceBlock() const;
    //void HandleMouseInput();
    //void HandleKeyboardMovementInput();
public:
    PlayerRenderer(const PlayerState& playerState, const PlayerState& oldPlayerState);
    //[[nodiscard]] float GetCameraPitch() const;
    //[[nodiscard]] bool GetMode() const;
    //[[nodiscard]] EBlockType GetCurrentSelectedBlock() const;
    //void UpdateMouseMove(float x, float y);
    //void KeyboardButtonPressed(int button, int action);
    //void MouseButtonPressed(int button, int action);
};
