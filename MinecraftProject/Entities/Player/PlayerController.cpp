//
// Created by amit on 4/22/2022.
//

#include "PlayerController.h"

using std::make_unique;

unique_ptr<Camera> CameraController::activeCamera = {};

void CameraController::setActiveCamera(Camera& camera) {
    activeCamera = make_unique<Camera>(camera);
}

Camera& CameraController::getActiveCamera() {
    return *activeCamera;
}

void CameraController::onResizeWindow(int width, int height) {
    activeCamera->setAspectRatio((float) width / (float) height);
}

PlayerController::PlayerController(uint16_t entityID, float x, float y, float z) : LivingEntity(entityID, playerSize,x,y,z), myCamera(CameraController::getActiveCamera()), mouseX(0), mouseY(0), prevMouseX(0), prevMouseY(0) {
    myCamera.setCameraPosition(vec3(x,y,z));
}

void PlayerController::tick() {
    handlePlayerInputs();
    //LivingEntity::tick();
}

void PlayerController::handlePlayerInputs() {
    myCamera.yaw += mouseX * mouseSensitivity;
    myCamera.pitch += -mouseY * mouseSensitivity;
    mouseX=0;
    mouseY=0;
}

void PlayerController::handleMouseMovementInput(float x, float y) {
    mouseX = x-prevMouseX;
    mouseY = y-prevMouseY;
    prevMouseX = x;
    prevMouseY = y;
}
