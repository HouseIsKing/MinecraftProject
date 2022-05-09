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
    activeCamera->SetAspectRatio((float) width / (float) height);
}

PlayerController::PlayerController(uint16_t entityID, float x, float y, float z) : LivingEntity(entityID, playerSize,x,y,z), myCamera(CameraController::getActiveCamera()), mouseX(0), mouseY(0), prevMouseX(0), prevMouseY(0) {
    myCamera.SetCameraPosition(vec3(x,y,z));
}

void PlayerController::tick() {
    handlePlayerInputs();
    LivingEntity::tick();
	//std::cout << getPos().x << " " << getPos().y << " " << getPos().z << std::endl;
	vec3 finalCameraPosition = getTransform().getPosition() + playerSize;
	finalCameraPosition.y += cameraOffset - playerSize.y;
	myCamera.SetCameraPosition(finalCameraPosition);
	EngineDefaults::getShader()->setMat4(EngineDefaults::getShader()->getUniformInt("view"), myCamera.GetViewMatrix());
	EngineDefaults::getShader()->setMat4(EngineDefaults::getShader()->getUniformInt("projection"), myCamera.GetProjectionMatrix());
}

void PlayerController::handlePlayerInputs() {
    myCamera.Yaw += mouseX * mouseSensitivity;
    myCamera.Pitch += -mouseY * mouseSensitivity;
	if (myCamera.Pitch > 89.0f)
		myCamera.Pitch = 89.0f;
	if(myCamera.Pitch < -89.0f)
		myCamera.Pitch = -89.0f;
	getTransform().setRotation(myCamera.Pitch, myCamera.Yaw,0);
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
