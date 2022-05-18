//
// Created by amit on 4/22/2022.
//

#include "PlayerController.h"
#include "../../Util/EngineDefaults.h"
#include <GLFW/glfw3.h>

#include "World/SinglePlayerWorld.h"

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
	MyCamera.Position = vec3(x, y, z);
}

void PlayerController::Tick()
{
	HandleMouseMovementInput();
	HandlePlayerInputs();
	LivingEntity::Tick();
	vec3 finalCameraPosition = GetTransform().GetPosition() + PLAYER_SIZE;
	finalCameraPosition.y += CAMERA_OFFSET - PLAYER_SIZE.y;
	MyCamera.Position = finalCameraPosition;
	Shader::SetMat4(EngineDefaults::GetShader()->GetUniformInt("view"), MyCamera.GetViewMatrix());
	Shader::SetMat4(EngineDefaults::GetShader()->GetUniformInt("projection"), MyCamera.GetProjectionMatrix());
}

void PlayerController::Render()
{
	LivingEntity::Render();
	DisplaySelectionHighlight();
}

void PlayerController::HandlePlayerInputs()
{
	HandleKeyboardMovementInput();
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
	GetTransform().SetRotation(0, MyCamera.Yaw, 0);
	MouseX = 0;
	MouseY = 0;
}

float PlayerController::GetSelectionHighlightBrightness(const int x, const int y, const int z, const BlockFaces face)
{
	switch (face)
	{
	case BlockFaces::Top:
		return GetWorld()->GetBrightnessAt(x, y + 1, z);
	case BlockFaces::Bottom:
		return GetWorld()->GetBrightnessAt(x, y - 1, z);
	case BlockFaces::North:
		return GetWorld()->GetBrightnessAt(x, y, z + 1);
	case BlockFaces::South:
		return GetWorld()->GetBrightnessAt(x, y, z - 1);
	case BlockFaces::East:
		return GetWorld()->GetBrightnessAt(x + 1, y, z);
	case BlockFaces::West:
		return GetWorld()->GetBrightnessAt(x - 1, y, z);
	}
	return 0;
}

void PlayerController::DisplaySelectionHighlight()
{
	//find closest face
	glm::ivec3 pos;
	bool found;
	const BlockFaces face = FindClosestFace(pos, found);
	const float brightness = GetSelectionHighlightBrightness(pos.x, pos.y, pos.z, face);
	if (found)
	{
		SelectionHighlight.Render(static_cast<float>(pos.x), static_cast<float>(pos.y), static_cast<float>(pos.z), face, brightness, *GetWorld()->GetBlockAt(pos.x, pos.y, pos.z));
	}
}

BlockFaces PlayerController::FindClosestFace(glm::ivec3& blockPosition, bool& foundBlock) const
{
	const vec3 frontVector = MyCamera.GetFrontVector();
	const vec3 cameraPos = MyCamera.Position;
	const bool right = frontVector.x > 0.0F;
	const bool up = frontVector.y > 0.0F;
	const bool forward = frontVector.z > 0.0F;
	float totalDistance = 0.0F;
	const float maxDistance = CalculateMaxDistanceForHighlight(frontVector, up, right, forward);
	float xDistance = cameraPos.x;
	float yDistance = cameraPos.y;
	float zDistance = cameraPos.z;
	while (totalDistance <= maxDistance)
	{
		const float distanceForX = ((right ? floor(xDistance) : ceil(xDistance)) - xDistance + (right ? 1.0F : -1.0F)) / frontVector.x;
		const float distanceForY = ((up ? floor(yDistance) : ceil(yDistance)) - yDistance + (up ? 1.0F : -1.0F)) / frontVector.y;
		const float distanceForZ = ((forward ? floor(zDistance) : ceil(zDistance)) - zDistance + (forward ? 1.0F : -1.0F)) / frontVector.z;
		const float distanceForXAbs = abs(distanceForX);
		const float distanceForYAbs = abs(distanceForY);
		const float distanceForZAbs = abs(distanceForZ);
		float minDistance;
		int xyzChoice;
		if (distanceForXAbs < distanceForYAbs && distanceForXAbs < distanceForZAbs)
		{
			minDistance = distanceForX;
			xyzChoice = 0;
		}
		else if (distanceForYAbs < distanceForZAbs)
		{
			minDistance = distanceForY;
			xyzChoice = 1;
		}
		else
		{
			minDistance = distanceForZ;
			xyzChoice = 2;
		}
		if (totalDistance + abs(minDistance) <= maxDistance)
		{
			xDistance += frontVector.x * minDistance;
			yDistance += frontVector.y * minDistance;
			zDistance += frontVector.z * minDistance;
			blockPosition = vec3(static_cast<int>(xDistance) - (!right && xyzChoice == 0 ? 1 : 0), static_cast<int>(yDistance) - (!up && xyzChoice == 1 ? 1 : 0), static_cast<int>(zDistance) - (!forward && xyzChoice == 2 ? 1 : 0));
			if (GetWorld()->IsBlockExists(blockPosition.x, blockPosition.y, blockPosition.z))
			{
				foundBlock = true;
				if (xyzChoice == 0 && frontVector.x > 0.0F)
				{
					return BlockFaces::West;
				}
				if (xyzChoice == 0 && frontVector.x < 0.0F)
				{
					return BlockFaces::East;
				}
				if (xyzChoice == 1 && frontVector.y > 0.0F)
				{
					return BlockFaces::Bottom;
				}
				if (xyzChoice == 1 && frontVector.y < 0.0F)
				{
					return BlockFaces::Top;
				}
				if (xyzChoice == 2 && frontVector.z > 0.0F)
				{
					return BlockFaces::South;
				}
				if (xyzChoice == 2 && frontVector.z < 0.0F)
				{
					return BlockFaces::North;
				}
			}
		}
		totalDistance += abs(minDistance);
	}
	foundBlock = false;
	return BlockFaces::Bottom;
}

float PlayerController::CalculateMaxDistanceForHighlight(const vec3& front, const bool up, const bool right, const bool forward) const
{
	float xDistance = right ? 4.0F : 3.0F;
	float yDistance = up ? 3.28F : 3.72F;
	float zDistance = forward ? 4.0F : 3.0F;
	xDistance /= front.x;
	yDistance /= front.y;
	zDistance /= front.z;
	xDistance = abs(xDistance);
	yDistance = abs(yDistance);
	zDistance = abs(zDistance);
	return glm::min(xDistance, glm::min(yDistance, zDistance));
}

void PlayerController::HandleMouseMovementInput()
{
	double x;
	double y;
	glfwGetCursorPos(GetWorld()->GetWindow(), &x, &y);
	MouseX = static_cast<float>(x) - PrevMouseX;
	MouseY = static_cast<float>(y) - PrevMouseY;
	PrevMouseX = static_cast<float>(x);
	PrevMouseY = static_cast<float>(y);
	HandlePlayerInputs();
}

void PlayerController::HandleKeyboardMovementInput()
{
	VerticalInput = 0;
	HorizontalInput = 0;
	JumpRequested = false;
	GLFWwindow* window = GetWorld()->GetWindow();
	int state = glfwGetKey(window, GLFW_KEY_W);
	if (state == GLFW_PRESS)
	{
		VerticalInput++;
	}
	if (state == GLFW_RELEASE)
	{
		VerticalInput--;
	}
	state = glfwGetKey(window, GLFW_KEY_S);
	if (state == GLFW_PRESS)
	{
		VerticalInput--;
	}
	if (state == GLFW_RELEASE)
	{
		VerticalInput++;
	}
	state = glfwGetKey(window, GLFW_KEY_A);
	if (state == GLFW_PRESS)
	{
		HorizontalInput--;
	}
	if (state == GLFW_RELEASE)
	{
		HorizontalInput++;
	}
	state = glfwGetKey(window, GLFW_KEY_D);
	if (state == GLFW_PRESS)
	{
		HorizontalInput++;
	}
	if (state == GLFW_RELEASE)
	{
		HorizontalInput--;
	}
	state = glfwGetKey(window, GLFW_KEY_SPACE);
	if (state == GLFW_PRESS)
	{
		JumpRequested = true;
	}
	if (state == GLFW_RELEASE)
	{
		JumpRequested = false;
	}
}
