//
// Created by amit on 4/22/2022.
//

#ifndef MINECRAFTPROJECT_PLAYERCONTROLLER_H
#define MINECRAFTPROJECT_PLAYERCONTROLLER_H
#include "Camera.h"
#include <memory>
#include "../LivingEntity.h"

using std::unique_ptr;

class CameraController {
private:
    static unique_ptr<Camera> activeCamera;
public:
    static void setActiveCamera(Camera& camera);
    static Camera& getActiveCamera();
    static void onResizeWindow(int width, int height);
};

class PlayerController : LivingEntity {
private:
    Camera& myCamera;
    constexpr static const vec3 playerSize = vec3(0.3f, 0.9f, 0.3f);
    constexpr static const float cameraOffset = 1.62f;
    float mouseX;
    float mouseY;
    float prevMouseX;
    float prevMouseY;
    float mouseSensitivity = 0.2f;
    void handlePlayerInputs();
public:
    PlayerController(uint16_t entityID, float x, float y, float z);
    void handleMouseMovementInput(float x, float y);
    void handleKeyboardMovementInput(int key, int action);
    void tick() override;
};


#endif //MINECRAFTPROJECT_PLAYERCONTROLLER_H
