#include "PlayerRenderer.h"
/*
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

void PlayerController::DisplaySelectionHighlight()
{
    //find closest face
    const int brightness = GetSelectionHighlightBrightness(SelectionHighlight.HitPosition.x, SelectionHighlight.HitPosition.y, SelectionHighlight.HitPosition.z, SelectionHighlight.FaceHit);
    if (SelectionHighlight.BlockHit != nullptr)
    {
        SelectionHighlight.Render(brightness);
    }
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

void PlayerController::HandleMouseInput()
{
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
}*/

PlayerRenderer::PlayerRenderer(const PlayerState& playerState, const PlayerState& oldPlayerState) : LivingEntityRenderer(playerState, oldPlayerState)
{
}
