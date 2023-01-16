#pragma once
#include <memory>

#include "Camera.h"

class CameraController
{
    static std::unique_ptr<Camera> ActiveCamera;

public:
    static void SetActiveCamera(Camera& camera);
    static Camera& GetActiveCamera();
    static void OnResizeWindow(int width, int height);
};
