#include "CameraController.h"

std::unique_ptr<Camera> CameraController::ActiveCamera = {};

void CameraController::SetActiveCamera(Camera& camera)
{
    ActiveCamera = std::make_unique<Camera>(camera);
}

Camera& CameraController::GetActiveCamera()
{
    return *ActiveCamera;
}

void CameraController::OnResizeWindow(const int width, const int height)
{
    ActiveCamera->SetAspectRatio(static_cast<float>(width) / static_cast<float>(height));
}
