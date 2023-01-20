#include "PlayerController.h"
#include "Entities/Generic/CameraController.h"
#include "Entities/Generic/Zombie.h"
#include "Util/EngineDefaults.h"
#include "World/SP/SinglePlayerWorld.h"

PlayerController::PlayerController(const float x, const float y, const float z) : LivingEntity(PLAYER_SIZE, x, y, z),
    MyCamera(CameraController::GetActiveCamera()), LeftMousePressed(false), RightMousePressed(false), PrevMouseX(0),
    PrevMouseY(0), DeltaMouseX(0), DeltaMouseY(0), PrevPitch(0), IsSpawnZombieButtonPressed(false),
    CurrentSelectedBlock(EBlockType::Stone),
    SelectedBlockGuiPtr(nullptr), SelectionHighlight(this)
{
    double xMouse;
    double yMouse;
    glfwGetCursorPos(GetWorld()->GetWindow(), &xMouse, &yMouse);
    PrevMouseX = static_cast<float>(xMouse);
    PrevMouseY = static_cast<float>(yMouse);
    MyCamera.Position = glm::vec3(x, y, z);
}

void PlayerController::Render(const float partialTick)
{
    const glm::vec3 pos = GetTransform().GetPosition();
    const glm::vec3 rot = GetTransform().GetRotation();
    glm::vec3 finalCameraPosition = PrevTransform.GetPosition() + (pos - PrevTransform.GetPosition()) * partialTick;
    glm::vec3 finalCameraRotation = PrevTransform.GetRotation() + (rot - PrevTransform.GetRotation()) * partialTick;
    GetTransform().SetPosition(finalCameraPosition);
    GetTransform().SetRotation(finalCameraRotation);
    finalCameraRotation.x = PrevPitch + (MyCamera.Pitch - PrevPitch) * partialTick;
    const float tempPitch = MyCamera.Pitch;
    MyCamera.Pitch = finalCameraRotation.x;
    MyCamera.Yaw = finalCameraRotation.y;
    LivingEntity::Render(partialTick);
    finalCameraPosition.y += CAMERA_OFFSET - PLAYER_SIZE.y;
    MyCamera.Position = finalCameraPosition;
    Shader::SetMat4(EngineDefaults::GetShader()->GetUniformInt("view"), MyCamera.GetViewMatrix());
    Shader::SetMat4(EngineDefaults::GetShader()->GetUniformInt("projection"), MyCamera.GetProjectionMatrix());
    GetTransform().SetPosition(pos);
    GetTransform().SetRotation(rot);
    MyCamera.Pitch = tempPitch;
    MyCamera.Yaw = rot.y;
    if (SelectedBlockGuiPtr == nullptr)
    {
        SelectedBlockGuiPtr = dynamic_cast<SelectedBlockGui<SinglePlayerWorld>*>(GetWorld()->GetGuiOfType<SelectedBlockGui<SinglePlayerWorld>>());
    }
    SelectionHighlight.Reset();
    bool found = false;
    SelectionHighlight.FaceHit = FindClosestFace(SelectionHighlight.HitPosition, found);
    SelectionHighlight.BlockHit = found ? GetWorld()->GetBlockAt(SelectionHighlight.HitPosition.x, SelectionHighlight.HitPosition.y, SelectionHighlight.HitPosition.z) : nullptr;
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
            if (blockToPlace->IsSolidBlock() && box.IsIntersecting(GetBoundingBox()))
            {
                break;
            }
            GetWorld()->PlaceBlockAt(x, y - 1, z, CurrentSelectedBlock);
            break;
        case BlockFaces::Top:
            box.Move(static_cast<float>(x), static_cast<float>(y + 1), static_cast<float>(z));
            if (blockToPlace->IsSolidBlock() && box.IsIntersecting(GetBoundingBox()))
            {
                break;
            }
            GetWorld()->PlaceBlockAt(x, y + 1, z, CurrentSelectedBlock);
            break;
        case BlockFaces::North:
            box.Move(static_cast<float>(x), static_cast<float>(y), static_cast<float>(z + 1));
            if (blockToPlace->IsSolidBlock() && box.IsIntersecting(GetBoundingBox()))
            {
                break;
            }
            GetWorld()->PlaceBlockAt(x, y, z + 1, CurrentSelectedBlock);
            break;
        case BlockFaces::South:
            box.Move(static_cast<float>(x), static_cast<float>(y), static_cast<float>(z - 1));
            if (blockToPlace->IsSolidBlock() && box.IsIntersecting(GetBoundingBox()))
            {
                break;
            }
            GetWorld()->PlaceBlockAt(x, y, z - 1, CurrentSelectedBlock);
            break;
        case BlockFaces::East:
            box.Move(static_cast<float>(x + 1), static_cast<float>(y), static_cast<float>(z));
            if (blockToPlace->IsSolidBlock() && box.IsIntersecting(GetBoundingBox()))
            {
                break;
            }
            GetWorld()->PlaceBlockAt(x + 1, y, z, CurrentSelectedBlock);
            break;
        case BlockFaces::West:
            box.Move(static_cast<float>(x - 1), static_cast<float>(y), static_cast<float>(z));
            if (blockToPlace->IsSolidBlock() && box.IsIntersecting(GetBoundingBox()))
            {
                break;
            }
            GetWorld()->PlaceBlockAt(x - 1, y, z, CurrentSelectedBlock);
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
    DeltaMouseX += x - PrevMouseX;
    DeltaMouseY += y - PrevMouseY;
    PrevMouseX = x;
    PrevMouseY = y;
}

EEntityType PlayerController::GetEntityType() const
{
    return EEntityType::Player;
}

void PlayerController::HandleMouseInput()
{
    PrevPitch = MyCamera.Pitch;
    MyCamera.Yaw += DeltaMouseX * MouseSensitivity;
    MyCamera.Pitch += -DeltaMouseY * MouseSensitivity;
    DeltaMouseX = 0.0F;
    DeltaMouseY = 0.0F;
    if (MyCamera.Pitch > 89.0F)
    {
        MyCamera.Pitch = 89.0F;
    }
    if (MyCamera.Pitch < -89.0F)
    {
        MyCamera.Pitch = -89.0F;
    }
    GetTransform().SetRotation(0, MyCamera.Yaw, 0);
    int state = glfwGetMouseButton(GetWorld()->GetWindow(), GLFW_MOUSE_BUTTON_LEFT);
    if (state == GLFW_PRESS && !LeftMousePressed)
    {
        LeftMousePressed = true;
        if (Mode)
        {
            PlaceBlock();
        }
        else
        {
            if (SelectionHighlight.BlockHit != nullptr)
            {
                GetWorld()->RemoveBlockAt(SelectionHighlight.HitPosition.x, SelectionHighlight.HitPosition.y, SelectionHighlight.HitPosition.z);
            }
        }
    }
    else if (state == GLFW_RELEASE)
    {
        LeftMousePressed = false;
    }
    state = glfwGetMouseButton(GetWorld()->GetWindow(), GLFW_MOUSE_BUTTON_RIGHT);
    if (state == GLFW_PRESS && !RightMousePressed)
    {
        Mode = !Mode;
        RightMousePressed = true;
    }
    else if (state == GLFW_RELEASE)
    {
        RightMousePressed = false;
    }
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
    state = glfwGetKey(window, GLFW_KEY_S);
    if (state == GLFW_PRESS)
    {
        VerticalInput--;
    }
    state = glfwGetKey(window, GLFW_KEY_A);
    if (state == GLFW_PRESS)
    {
        HorizontalInput--;
    }
    state = glfwGetKey(window, GLFW_KEY_D);
    if (state == GLFW_PRESS)
    {
        HorizontalInput++;
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
    state = glfwGetKey(window, GLFW_KEY_1);
    if (state == GLFW_PRESS)
    {
        CurrentSelectedBlock = EBlockType::Stone;
    }
    state = glfwGetKey(window, GLFW_KEY_2);
    if (state == GLFW_PRESS)
    {
        CurrentSelectedBlock = EBlockType::Dirt;
    }
    state = glfwGetKey(window, GLFW_KEY_3);
    if (state == GLFW_PRESS)
    {
        CurrentSelectedBlock = EBlockType::Cobblestone;
    }
    state = glfwGetKey(window, GLFW_KEY_4);
    if (state == GLFW_PRESS)
    {
        CurrentSelectedBlock = EBlockType::Planks;
    }
    state = glfwGetKey(window, GLFW_KEY_5);
    if (state == GLFW_PRESS)
    {
        CurrentSelectedBlock = EBlockType::Sapling;
    }
    if (SelectedBlockGuiPtr != nullptr)
    {
        SelectedBlockGuiPtr->SwitchBlockType(CurrentSelectedBlock);
    }
    state = glfwGetKey(window, GLFW_KEY_G);
    if (state == GLFW_PRESS && !IsSpawnZombieButtonPressed)
    {
        const glm::vec3 pos = GetTransform().GetPosition();
        new Zombie<SinglePlayerWorld>(pos.x, pos.y, pos.z);
        IsSpawnZombieButtonPressed = true;
    }
    if (state == GLFW_RELEASE)
    {
        IsSpawnZombieButtonPressed = false;
    }
    state = glfwGetKey(window, GLFW_KEY_R);
    if (state == GLFW_PRESS)
    {
        GetTransform().SetPosition(EngineDefaults::GetNextFloat() * 256.0F, 67.0F, EngineDefaults::GetNextFloat() * 256.0F);
    }
}
