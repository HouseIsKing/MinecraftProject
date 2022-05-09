//
// Created by amit on 4/22/2022.
//

#include "PlayerController.h"
#include "../../Util/EngineDefaults.h"
#include <GLFW/glfw3.h>

using std::make_unique;

unique_ptr<Camera> CameraController::ActiveCamera = {};

void CameraController::SetActiveCamera(Camera& camera)
{
	ActiveCamera = make_unique<Camera>(camera);
}

Camera& CameraController::GetActiveCamera()
{
	return *ActiveCamera;
}

void CameraController::OnResizeWindow(const int width, const int height)
{
	ActiveCamera->SetAspectRatio(static_cast<float>(width) / static_cast<float>(height));
}

PlayerController::PlayerController(const uint16_t entityId, const float x, const float y, const float z) : LivingEntity(entityId, PLAYER_SIZE, x, y, z), MyCamera(CameraController::GetActiveCamera()), MouseX(0), MouseY(0), PrevMouseX(0), PrevMouseY(0)
{
	MyCamera.SetCameraPosition(vec3(x, y, z));
}

void PlayerController::Tick()
{
	HandlePlayerInputs();
	LivingEntity::Tick();
	vec3 finalCameraPosition = GetTransform().GetPosition() + PLAYER_SIZE;
	finalCameraPosition.y += CAMERA_OFFSET - PLAYER_SIZE.y;
	MyCamera.SetCameraPosition(finalCameraPosition);
	EngineDefaults::GetShader()->setMat4(EngineDefaults::GetShader()->getUniformInt("view"), MyCamera.GetViewMatrix());
	EngineDefaults::GetShader()->setMat4(EngineDefaults::GetShader()->getUniformInt("projection"), MyCamera.GetProjectionMatrix());
}

void PlayerController::HandlePlayerInputs()
{
	MyCamera.Yaw += MouseX * MouseSensitivity;
	MyCamera.Pitch += -MouseY * MouseSensitivity;
	if (MyCamera.Pitch > 89.0F)
	{
		MyCamera.Pitch = 89.0F;
	}
	if (MyCamera.Pitch < -89.0F)
	{
		MyCamera.Pitch = -89.0F;
	}
	GetTransform().SetRotation(MyCamera.Pitch, MyCamera.Yaw, 0);
	MouseX = 0;
	MouseY = 0;
}

void PlayerController::HandleMouseMovementInput(const float x, const float y)
{
	MouseX = x - PrevMouseX;
	MouseY = y - PrevMouseY;
	PrevMouseX = x;
	PrevMouseY = y;
}

void PlayerController::HandleKeyboardMovementInput(const int key, const int action)
{
	if (action == GLFW_PRESS)
	{
		if (key == GLFW_KEY_W)
		{
			VerticalInput++;
		}
		else if (key == GLFW_KEY_S)
		{
			VerticalInput--;
		}
		else if (key == GLFW_KEY_A)
		{
			HorizontalInput--;
		}
		else if (key == GLFW_KEY_D)
		{
			HorizontalInput++;
		}
		else if (key == GLFW_KEY_SPACE)
		{
			JumpRequested = true;
		}
	}
	if (action == GLFW_RELEASE)
	{
		if (key == GLFW_KEY_W)
		{
			VerticalInput--;
		}
		else if (key == GLFW_KEY_S)
		{
			VerticalInput++;
		}
		else if (key == GLFW_KEY_A)
		{
			HorizontalInput++;
		}
		else if (key == GLFW_KEY_D)
		{
			HorizontalInput--;
		}
	}
}
