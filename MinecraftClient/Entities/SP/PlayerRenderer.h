#pragma once
#include "Entities/Generic/LivingEntityRenderer.h"
#include "Util/States/PlayerState.h"


class PlayerRenderer : public LivingEntityRenderer<PlayerState>
{
    //EBlockType CurrentSelectedBlock;
    //SelectedBlockGui* SelectedBlockGuiPtr;
    //static int GetSelectionHighlightBrightness(int x, int y, int z, BlockFaces face);
    //void PlaceBlock() const;
    //void HandleMouseInput();
    //void HandleKeyboardMovementInput();
public:
    PlayerRenderer(const PlayerState& playerState, const PlayerState& oldPlayerState);
    //[[nodiscard]] bool GetMode() const;
    //[[nodiscard]] EBlockType GetCurrentSelectedBlock() const;
    //void UpdateMouseMove(float x, float y);
    //void KeyboardButtonPressed(int button, int action);
    //void MouseButtonPressed(int button, int action);
};
