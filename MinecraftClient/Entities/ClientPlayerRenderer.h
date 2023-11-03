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
    explicit ClientPlayerRenderer(uint16_t entityId);
    [[nodiscard]] Frustum GetCameraFrustum() const;
    void Render(float partialTick) override;
    void RenderSelectionHighlight();
};
