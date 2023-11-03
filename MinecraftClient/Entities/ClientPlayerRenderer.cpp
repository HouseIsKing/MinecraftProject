#include "ClientPlayerRenderer.h"
#include "Generic/CameraController.h"
#include "Util/EngineDefaultsClient.h"
#include "World/Generic/ClientWorld.h"

ClientPlayerRenderer::ClientPlayerRenderer(const uint16_t entityId) : PlayerRenderer(entityId), MyCamera(CameraController::GetActiveCamera()), SelectionHighlight(entityId)
{
    const PlayerState& playerState = World::GetWorld()->GetEntityState<PlayerStateWrapper, PlayerState>(entityId).first;
    BlockGui = new SelectedBlockGui(playerState);
    ClientWorld::GetWorld()->AddGui(BlockGui);
    BlockGui->Active = true;
    BlockGui->Rebuild();
}

Frustum ClientPlayerRenderer::GetCameraFrustum() const
{
    return MyCamera.GetFrustum();
}

void ClientPlayerRenderer::Render(const float partialTick)
{
    const std::pair<const PlayerState&, const PlayerState&> states = World::GetWorld()->GetEntityState<PlayerStateWrapper, PlayerState>(EntityId);
    if (states.second.CurrentSelectedBlock != states.first.CurrentSelectedBlock)
    {
        BlockGui->Rebuild();
    }
    PlayerRenderer::Render(partialTick);
    glm::vec3 finalCameraPosition = states.second.EntityTransform.Position + (states.first.EntityTransform.Position - states.second.EntityTransform.Position) * partialTick;
    glm::vec3 finalCameraRotation = states.second.EntityTransform.Rotation + (states.first.EntityTransform.Rotation - states.second.EntityTransform.Rotation) * partialTick;
    Transform.Position = finalCameraPosition;
    Transform.Rotation = finalCameraRotation;
    finalCameraRotation.x = states.second.CameraPitch + (states.first.CameraPitch - states.second.CameraPitch) * partialTick;
    MyCamera.Pitch = finalCameraRotation.x;
    MyCamera.Yaw = finalCameraRotation.y;
    finalCameraPosition.y += EngineDefaults::CAMERA_OFFSET - EngineDefaults::PLAYER_SIZE.y;
    MyCamera.Position = finalCameraPosition;
    Shader::SetMat4(EngineDefaultsClient::GetShader()->GetUniformInt("view"), MyCamera.GetViewMatrix());
    Shader::SetMat4(EngineDefaultsClient::GetShader()->GetUniformInt("projection"), MyCamera.GetProjectionMatrix());
}

void ClientPlayerRenderer::RenderSelectionHighlight()
{
    SelectionHighlight.Render();
}
