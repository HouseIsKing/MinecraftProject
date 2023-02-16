#include "ClientPlayerRenderer.h"
#include "Generic/CameraController.h"
#include "Util/EngineDefaultsClient.h"
#include "World/Generic/ClientWorld.h"

ClientPlayerRenderer::ClientPlayerRenderer(const PlayerState& playerState, const PlayerState& oldPlayerState) : PlayerRenderer(playerState, oldPlayerState), MyCamera(CameraController::GetActiveCamera()), SelectionHighlight(playerState), BlockGui(new SelectedBlockGui(playerState))
{
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
    PlayerRenderer::Render(partialTick);
    glm::vec3 finalCameraPosition = OldState.EntityTransform.Position + (State.EntityTransform.Position - OldState.EntityTransform.Position) * partialTick;
    glm::vec3 finalCameraRotation = OldState.EntityTransform.Rotation + (State.EntityTransform.Rotation - OldState.EntityTransform.Rotation) * partialTick;
    Transform.Position = finalCameraPosition;
    Transform.Rotation = finalCameraRotation;
    finalCameraRotation.x = OldState.CameraPitch + (State.CameraPitch - OldState.CameraPitch) * partialTick;
    MyCamera.Pitch = finalCameraRotation.x;
    MyCamera.Yaw = finalCameraRotation.y;
    finalCameraPosition.y += EngineDefaults::CAMERA_OFFSET - EngineDefaults::PLAYER_SIZE.y;
    MyCamera.Position = finalCameraPosition;
    Shader::SetMat4(EngineDefaultsClient::GetShader()->GetUniformInt("view"), MyCamera.GetViewMatrix());
    Shader::SetMat4(EngineDefaultsClient::GetShader()->GetUniformInt("projection"), MyCamera.GetProjectionMatrix());
}

void ClientPlayerRenderer::Changed()
{
    if (OldState.CurrentSelectedBlock != State.CurrentSelectedBlock)
    {
        BlockGui->Rebuild();
    }
}

void ClientPlayerRenderer::RenderSelectionHighlight()
{
    SelectionHighlight.Render();
}
