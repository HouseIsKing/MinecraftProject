#pragma once
#include "Generic/Camera.h"
#include "SP/PlayerRenderer.h"

class ClientPlayerRenderer final : public PlayerRenderer
{
    Camera& MyCamera;

public:
    ClientPlayerRenderer(const PlayerState& playerState, const PlayerState& oldPlayerState);
    [[nodiscard]] Frustum GetCameraFrustum() const;
    void Render(float partialTick) override;
};
