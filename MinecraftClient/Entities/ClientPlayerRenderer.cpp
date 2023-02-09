#include "ClientPlayerRenderer.h"
#include "Generic/CameraController.h"
#include "Util/EngineDefaultsClient.h"

ClientPlayerRenderer::ClientPlayerRenderer(const PlayerState& playerState, const PlayerState& oldPlayerState) : PlayerRenderer(playerState, oldPlayerState), MyCamera(CameraController::GetActiveCamera())
{
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
    /*SelectionHighlight.Reset();
    bool found = false;
    SelectionHighlight.FaceHit = FindClosestFace(SelectionHighlight.HitPosition, found);
    SelectionHighlight.BlockHit = found ? TheWorld->GetBlockAt(SelectionHighlight.HitPosition.x, SelectionHighlight.HitPosition.y, SelectionHighlight.HitPosition.z) : nullptr;*/
}
