//
// Created by amit on 4/22/2022.
//

#include "PlayerController.h"
#include <GLFW/glfw3.h>

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
    LivingEntity::tick();
	//std::cout << getPos().x << " " << getPos().y << " " << getPos().z << std::endl;
	vec3 finalCameraPosition = getTransform().getPosition() + playerSize;
	finalCameraPosition.y += cameraOffset - playerSize.y;
	myCamera.setCameraPosition(finalCameraPosition);
}

void PlayerController::handlePlayerInputs() {
    myCamera.yaw += mouseX * mouseSensitivity;
    myCamera.pitch += -mouseY * mouseSensitivity;
	if (myCamera.pitch > 89.0f)
		myCamera.pitch = 89.0f;
	if(myCamera.pitch < -89.0f)
		myCamera.pitch = -89.0f;
	getTransform().setRotation(myCamera.pitch, myCamera.yaw,0);
    mouseX=0;
    mouseY=0;
}

void PlayerController::handleMouseMovementInput(float x, float y) {
    mouseX = x-prevMouseX;
    mouseY = y-prevMouseY;
    prevMouseX = x;
    prevMouseY = y;
}

void PlayerController::handleKeyboardMovementInput(int key, int action)
{
    if (action == GLFW_PRESS)
    {
        if (key == GLFW_KEY_W)
        {
			verticalInput++;
		}
		else if (key == GLFW_KEY_S)
		{
			verticalInput--;
		}
		else if (key == GLFW_KEY_A)
		{
			horizontalInput--;
		}
		else if (key == GLFW_KEY_D)
		{
			horizontalInput++;
		}
		else if (key == GLFW_KEY_SPACE)
		{
			jumpRequested = true;
		}
    }
	if (action == GLFW_RELEASE)
	{
		if (key == GLFW_KEY_W)
		{
			verticalInput--;
		}
		else if (key == GLFW_KEY_S)
		{
			verticalInput++;
		}
		else if (key == GLFW_KEY_A)
		{
			horizontalInput++;
		}
		else if (key == GLFW_KEY_D)
		{
			horizontalInput--;
		}
	}
}
