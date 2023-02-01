#include "PlayerController.h"

#include "World/Generic/World.h"

PlayerController::PlayerController(const float x, const float y, const float z) : LivingEntity(PLAYER_SIZE, x, y, z, new PlayerState(TheWorld->RegisterEntity(this))),
    MyCamera(CameraController::GetActiveCamera()), PrevMouseX(0), PrevMouseY(0), PrevPitch(0), CurrentSelectedBlock(EBlockType::Stone),
    SelectedBlockGuiPtr(TheWorld->GetGuiOfType<SelectedBlockGui>()), SelectionHighlight(this)
{
    double xMouse;
    double yMouse;
    glfwGetCursorPos(TheWorld->GetWindow(), &xMouse, &yMouse);
    PrevMouseX = static_cast<float>(xMouse);
    PrevMouseY = static_cast<float>(yMouse);
    MyCamera.Position = glm::vec3(x, y, z);
}


PlayerController::PlayerController(const PlayerState& playerState) : LivingEntity(playerState.EntityTransform.Scale, playerState.EntityTransform.Position.x, playerState.EntityTransform.Position.y, playerState.EntityTransform.Position.z, new PlayerState(playerState)),
                                                                     MyCamera(CameraController::GetActiveCamera()), PrevMouseX(0), PrevMouseY(0), PrevPitch(0), CurrentSelectedBlock(EBlockType::Stone),
                                                                     SelectedBlockGuiPtr(TheWorld->GetGuiOfType<SelectedBlockGui>()), SelectionHighlight(this)
{
}

void PlayerController::Render(const float partialTick)
{
    const glm::vec3 pos = this->GetTransform().Position;
    const glm::vec3 rot = this->GetTransform().Rotation;
    glm::vec3 finalCameraPosition = this->PrevTransform.Position + (pos - this->PrevTransform.Position) * partialTick;
    glm::vec3 finalCameraRotation = this->PrevTransform.Rotation + (rot - this->PrevTransform.Rotation) * partialTick;
    this->GetTransform().Position = finalCameraPosition;
    this->GetTransform().Rotation = finalCameraRotation;
    finalCameraRotation.x = PrevPitch + (MyCamera.Pitch - PrevPitch) * partialTick;
    const float tempPitch = MyCamera.Pitch;
    MyCamera.Pitch = finalCameraRotation.x;
    MyCamera.Yaw = finalCameraRotation.y;
    LivingEntity::Render(partialTick);
    finalCameraPosition.y += CAMERA_OFFSET - PLAYER_SIZE.y;
    MyCamera.Position = finalCameraPosition;
    Shader::SetMat4(EngineDefaults::GetShader()->GetUniformInt("view"), MyCamera.GetViewMatrix());
    Shader::SetMat4(EngineDefaults::GetShader()->GetUniformInt("projection"), MyCamera.GetProjectionMatrix());
    this->GetTransform().Position = pos;
    this->GetTransform().Rotation = rot;
    MyCamera.Pitch = tempPitch;
    MyCamera.Yaw = rot.y;
    SelectionHighlight.Reset();
    bool found = false;
    SelectionHighlight.FaceHit = FindClosestFace(SelectionHighlight.HitPosition, found);
    SelectionHighlight.BlockHit = found ? TheWorld->GetBlockAt(SelectionHighlight.HitPosition.x, SelectionHighlight.HitPosition.y, SelectionHighlight.HitPosition.z) : nullptr;
}

void PlayerController::Tick()
{
    LivingEntity::Tick();
    HandleMouseInput();
    HandleKeyboardMovementInput();
}

int PlayerController::GetSelectionHighlightBrightness(const int x, const int y, const int z, const BlockFaces face)
{
    switch (face)
    {
    case BlockFaces::Top:
        return TheWorld->GetBrightnessAt(x, y + 1, z);
    case BlockFaces::Bottom:
        return TheWorld->GetBrightnessAt(x, y - 1, z);
    case BlockFaces::North:
        return TheWorld->GetBrightnessAt(x, y, z + 1);
    case BlockFaces::South:
        return TheWorld->GetBrightnessAt(x, y, z - 1);
    case BlockFaces::East:
        return TheWorld->GetBrightnessAt(x + 1, y, z);
    case BlockFaces::West:
        return TheWorld->GetBrightnessAt(x - 1, y, z);
    }
    return 0;
}

void PlayerController::DisplaySelectionHighlight()
{
    //find closest face
    const int brightness = GetSelectionHighlightBrightness(SelectionHighlight.HitPosition.x, SelectionHighlight.HitPosition.y, SelectionHighlight.HitPosition.z, SelectionHighlight.FaceHit);
    if (SelectionHighlight.BlockHit != nullptr)
    {
        SelectionHighlight.Render(brightness);
    }
}

BlockFaces PlayerController::FindClosestFace(glm::ivec3& blockPosition, bool& foundBlock) const
{
    const glm::vec3 frontVector = MyCamera.GetFrontVector();
    const glm::vec3 cameraPos = MyCamera.Position;
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
        float distanceForX = ((right ? floor(xDistance) : ceil(xDistance)) - xDistance + (right ? 1.0F : -1.0F)) / frontVector.x;
        float distanceForY = ((up ? floor(yDistance) : ceil(yDistance)) - yDistance + (up ? 1.0F : -1.0F)) / frontVector.y;
        float distanceForZ = ((forward ? floor(zDistance) : ceil(zDistance)) - zDistance + (forward ? 1.0F : -1.0F)) / frontVector.z;
        float distanceForXAbs = abs(distanceForX);
        if (distanceForXAbs < 0.000001F)
        {
            distanceForXAbs = 1.0F;
            distanceForX += right ? 1.0F : -1.0F;
        }
        float distanceForYAbs = abs(distanceForY);
        if (distanceForYAbs < 0.000001F)
        {
            distanceForYAbs = 1.0F;
            distanceForY += up ? 1.0F : -1.0F;
        }
        float distanceForZAbs = abs(distanceForZ);
        if (distanceForZAbs < 0.000001F)
        {
            distanceForZAbs = 1.0F;
            distanceForZ += forward ? 1.0F : -1.0F;
        }
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
            blockPosition = glm::vec3(static_cast<int>(floor(xDistance)) - (!right && xyzChoice == 0 ? 1 : 0), static_cast<int>(floor(yDistance)) - (!up && xyzChoice == 1 ? 1 : 0), static_cast<int>(floor(zDistance)) - (!forward && xyzChoice == 2 ? 1 : 0));
            if (TheWorld->IsBlockExists(blockPosition.x, blockPosition.y, blockPosition.z))
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

float PlayerController::CalculateMaxDistanceForHighlight(const glm::vec3& front, const bool up, const bool right, const bool forward) const
{
    float xDistance = right ? 4.0F + PLAYER_SIZE.x : 3.0F + PLAYER_SIZE.x;
    float yDistance = up ? 4.0F + 2 * PLAYER_SIZE.y - CAMERA_OFFSET : 3.0F + CAMERA_OFFSET;
    float zDistance = forward ? 4.0F + PLAYER_SIZE.z : 3.0F + PLAYER_SIZE.z;
    xDistance /= front.x;
    yDistance /= front.y;
    zDistance /= front.z;
    xDistance = abs(xDistance);
    yDistance = abs(yDistance);
    zDistance = abs(zDistance);
    return glm::min(xDistance, glm::min(yDistance, zDistance));
}

void PlayerController::PlaceBlock() const
{
    if (SelectionHighlight.BlockHit != nullptr)
    {
        const int x = SelectionHighlight.HitPosition.x;
        const int y = SelectionHighlight.HitPosition.y;
        const int z = SelectionHighlight.HitPosition.z;
        const Block* blockToPlace = BlockTypeList::GetBlockTypeData(CurrentSelectedBlock);
        BoundingBox box = blockToPlace->GetBoundingBox();
        switch (SelectionHighlight.FaceHit)
        {
        case BlockFaces::Bottom:
            box.Move(static_cast<float>(x), static_cast<float>(y - 1), static_cast<float>(z));
            if (blockToPlace->IsSolidBlock() && box.IsIntersecting(this->GetBoundingBox()))
            {
                break;
            }
            TheWorld->PlaceBlockAt(x, y - 1, z, CurrentSelectedBlock);
            break;
        case BlockFaces::Top:
            box.Move(static_cast<float>(x), static_cast<float>(y + 1), static_cast<float>(z));
            if (blockToPlace->IsSolidBlock() && box.IsIntersecting(this->GetBoundingBox()))
            {
                break;
            }
            TheWorld->PlaceBlockAt(x, y + 1, z, CurrentSelectedBlock);
            break;
        case BlockFaces::North:
            box.Move(static_cast<float>(x), static_cast<float>(y), static_cast<float>(z + 1));
            if (blockToPlace->IsSolidBlock() && box.IsIntersecting(this->GetBoundingBox()))
            {
                break;
            }
            TheWorld->PlaceBlockAt(x, y, z + 1, CurrentSelectedBlock);
            break;
        case BlockFaces::South:
            box.Move(static_cast<float>(x), static_cast<float>(y), static_cast<float>(z - 1));
            if (blockToPlace->IsSolidBlock() && box.IsIntersecting(this->GetBoundingBox()))
            {
                break;
            }
            TheWorld->PlaceBlockAt(x, y, z - 1, CurrentSelectedBlock);
            break;
        case BlockFaces::East:
            box.Move(static_cast<float>(x + 1), static_cast<float>(y), static_cast<float>(z));
            if (blockToPlace->IsSolidBlock() && box.IsIntersecting(this->GetBoundingBox()))
            {
                break;
            }
            TheWorld->PlaceBlockAt(x + 1, y, z, CurrentSelectedBlock);
            break;
        case BlockFaces::West:
            box.Move(static_cast<float>(x - 1), static_cast<float>(y), static_cast<float>(z));
            if (blockToPlace->IsSolidBlock() && box.IsIntersecting(this->GetBoundingBox()))
            {
                break;
            }
            TheWorld->PlaceBlockAt(x - 1, y, z, CurrentSelectedBlock);
            break;
        }
    }
}

Frustum PlayerController::GetCameraFrustum() const
{
    return MyCamera.GetFrustum();
}

float PlayerController::GetCameraPitch() const
{
    return MyCamera.Pitch;
}

bool PlayerController::GetMode() const
{
    return Mode;
}

EBlockType PlayerController::GetCurrentSelectedBlock() const
{
    return CurrentSelectedBlock;
}

void PlayerController::UpdateMouseMove(const float x, const float y)
{
    InputState.MouseX += (x - PrevMouseX) * MouseSensitivity;
    InputState.MouseY += (y - PrevMouseY) * MouseSensitivity;
    PrevMouseX = x;
    PrevMouseY = y;
}

void PlayerController::KeyboardButtonPressed(const int button, const int action)
{
    switch (button)
    {
    case GLFW_KEY_W:
        if (action == GLFW_PRESS)
        {
            InputState.ForwardAxis++;
        }
        else if (action == GLFW_RELEASE)
        {
            InputState.ForwardAxis--;
        }
        break;
    case GLFW_KEY_S:
        if (action == GLFW_PRESS)
        {
            InputState.ForwardAxis--;
        }
        else if (action == GLFW_RELEASE)
        {
            InputState.ForwardAxis++;
        }
        break;
    case GLFW_KEY_A:
        if (action == GLFW_PRESS)
        {
            InputState.RightAxis--;
        }
        else if (action == GLFW_RELEASE)
        {
            InputState.RightAxis++;
        }
        break;
    case GLFW_KEY_D:
        if (action == GLFW_PRESS)
        {
            InputState.RightAxis++;
        }
        else if (action == GLFW_RELEASE)
        {
            InputState.RightAxis--;
        }
        break;
    case GLFW_KEY_SPACE:
        if (action == GLFW_PRESS)
        {
            InputState.JumpPressed = true;
        }
        else if (action == GLFW_RELEASE)
        {
            InputState.JumpPressed = false;
        }
        break;
    case GLFW_KEY_1:
        if (action == GLFW_PRESS)
        {
            InputState.OnePressed = true;
        }
        else if (action == GLFW_RELEASE)
        {
            InputState.OnePressed = false;
        }
        break;
    case GLFW_KEY_2:
        if (action == GLFW_PRESS)
        {
            InputState.TwoPressed = true;
        }
        else if (action == GLFW_RELEASE)
        {
            InputState.TwoPressed = false;
        }
        break;
    case GLFW_KEY_3:
        if (action == GLFW_PRESS)
        {
            InputState.ThreePressed = true;
        }
        else if (action == GLFW_RELEASE)
        {
            InputState.ThreePressed = false;
        }
        break;
    case GLFW_KEY_4:
        if (action == GLFW_PRESS)
        {
            InputState.FourPressed = true;
        }
        else if (action == GLFW_RELEASE)
        {
            InputState.FourPressed = false;
        }
        break;
    case GLFW_KEY_5:
        if (action == GLFW_PRESS)
        {
            InputState.FivePressed = true;
        }
        else if (action == GLFW_RELEASE)
        {
            InputState.FivePressed = false;
        }
        break;
    case GLFW_KEY_R:
        if (action == GLFW_PRESS)
        {
            InputState.ResetPressed = true;
        }
        else if (action == GLFW_RELEASE)
        {
            InputState.ResetPressed = false;
        }
        break;
    case GLFW_KEY_G:
        if (action == GLFW_PRESS)
        {
            InputState.SpawnZombiePressed = true;
        }
        else if (action == GLFW_RELEASE)
        {
            InputState.SpawnZombiePressed = false;
        }
        break;
    default:
        break;
    }
}

void PlayerController::MouseButtonPressed(const int button, const int action)
{
    switch (button)
    {
    case GLFW_MOUSE_BUTTON_LEFT:
        if (action == GLFW_PRESS)
        {
            InputState.LeftMouseButtonPressed = true;
        }
        else if (action == GLFW_RELEASE)
        {
            InputState.LeftMouseButtonPressed = false;
        }
        break;
    case GLFW_MOUSE_BUTTON_RIGHT:
        if (action == GLFW_PRESS)
        {
            InputState.RightMouseButtonPressed = true;
        }
        else if (action == GLFW_RELEASE)
        {
            InputState.RightMouseButtonPressed = false;
        }
        break;
    default:
        break;
    }
}

void PlayerController::HandleMouseInput()
{
    auto* playerState = reinterpret_cast<PlayerState*>(this->State.get());
    PrevPitch = MyCamera.Pitch;
    MyCamera.Yaw += InputState.MouseX;
    MyCamera.Pitch += -InputState.MouseY;
    if (MyCamera.Pitch > 89.0F)
    {
        MyCamera.Pitch = 89.0F;
    }
    if (MyCamera.Pitch < -89.0F)
    {
        MyCamera.Pitch = -89.0F;
    }
    this->GetTransform().Rotation.y = MyCamera.Yaw;
    playerState->CameraPitch = MyCamera.Pitch;
    if (InputState.LeftMouseButtonPressed)
    {
        if (Mode)
        {
            PlaceBlock();
        }
        else
        {
            if (SelectionHighlight.BlockHit != nullptr)
            {
                TheWorld->RemoveBlockAt(SelectionHighlight.HitPosition.x, SelectionHighlight.HitPosition.y, SelectionHighlight.HitPosition.z);
            }
        }
    }
    if (InputState.RightMouseButtonPressed)
    {
        Mode = !Mode;
    }
    InputState.MouseX = 0.0F;
    InputState.MouseY = 0.0F;
    InputState.LeftMouseButtonPressed = false;
    InputState.RightMouseButtonPressed = false;
}

void PlayerController::HandleKeyboardMovementInput()
{
    auto* playerState = reinterpret_cast<PlayerState*>(this->State.get());
    playerState->JumpRequested = InputState.JumpPressed;
    playerState->VerticalInput = InputState.ForwardAxis;
    playerState->HorizontalInput = InputState.RightAxis;
    const EBlockType previousSelectedBlock = CurrentSelectedBlock;
    if (InputState.OnePressed)
    {
        CurrentSelectedBlock = EBlockType::Stone;
    }
    if (InputState.TwoPressed)
    {
        CurrentSelectedBlock = EBlockType::Dirt;
    }
    if (InputState.ThreePressed)
    {
        CurrentSelectedBlock = EBlockType::Cobblestone;
    }
    if (InputState.FourPressed)
    {
        CurrentSelectedBlock = EBlockType::Planks;
    }
    if (InputState.FivePressed)
    {
        CurrentSelectedBlock = EBlockType::Sapling;
    }
    if (CurrentSelectedBlock != previousSelectedBlock)
    {
        SelectedBlockGuiPtr->SwitchBlockType(CurrentSelectedBlock);
    }
    if (InputState.SpawnZombiePressed)
    {
        const glm::vec3 pos = this->GetTransform().Position;
        new Zombie(pos.x, pos.y, pos.z);
    }
    if (InputState.ResetPressed)
    {
        this->GetTransform().Position = glm::vec3(TheWorld->RandomEngineState.GetNextFloat() * 256.0F, 67.0F, TheWorld->RandomEngineState.GetNextFloat() * 256.0F);
    }
    InputState.JumpPressed = false;
    InputState.SpawnZombiePressed = false;
    InputState.FivePressed = false;
    InputState.FourPressed = false;
    InputState.ThreePressed = false;
    InputState.TwoPressed = false;
    InputState.OnePressed = false;
    InputState.ResetPressed = false;
}
