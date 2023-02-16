#pragma once
#include "Generic/Camera.h"
#include "Generic/PlayerSelectionHighlight.h"
#include "GUI/SelectedBlockGui.h"
#include "SP/PlayerRenderer.h"

class ClientPlayerRenderer final : public PlayerRenderer
{
    Camera& MyCamera;
    PlayerSelectionHighlight SelectionHighlight;
    SelectedBlockGui* BlockGui;

public:
    ClientPlayerRenderer(const PlayerState& playerState, const PlayerState& oldPlayerState);
    [[nodiscard]] Frustum GetCameraFrustum() const;
    void Render(float partialTick) override;
    void Changed() override;
    void RenderSelectionHighlight();
};
