#include "Player.h"
//#include "Entities/Zombie.h"
#include "World/Blocks/BlockTypeList.h"
#include "World/World.h"
#include <algorithm>
#include <glm/common.hpp>

Player::Player(const float x, const float y, const float z, const uint16_t& id) : LivingEntity(EngineDefaults::PLAYER_SIZE, x, y, z, id)
{
}

Player::Player(const PlayerState& state) : LivingEntity(state)
{
}

void Player::Tick()
{
    LivingEntity::Tick();
    State.SetInputState(ClientInputs[World::GetWorld()->GetWorldTime() % EngineDefaults::ROLLBACK_COUNT]);
    HandleClientInput();
    //bool found = false;
    //FaceHit = FindClosestFace(found);
    //BlockHit = found ? World::GetWorld()->GetBlockAt(BlockHitPosition.x, BlockHitPosition.y, BlockHitPosition.z) : nullptr;
}

void Player::NewTick()
{
    SetClientInput(World::GetWorld()->GetWorldTime() + 1, {});
}

BlockFaces Player::FindClosestFace(bool& foundBlock, glm::ivec3& hitPosition) const
{
    const PlayerState& state = State.GetState();
    TransformStruct cameraTransform = state.EntityTransform;
    cameraTransform.Position += glm::vec3(0.0F, EngineDefaults::CAMERA_OFFSET - EngineDefaults::PLAYER_SIZE.y, 0.0F);
    cameraTransform.Rotation.x = state.CameraPitch;
    const glm::vec3 frontVector = cameraTransform.GetForwardVector();
    const bool right = frontVector.x > 0.0F;
    const bool up = frontVector.y > 0.0F;
    const bool forward = frontVector.z > 0.0F;
    float totalDistance = 0.0F;
    const float maxDistance = CalculateMaxDistanceForHighlight(frontVector, up, right, forward);
    float xDistance = cameraTransform.Position.x;
    float yDistance = cameraTransform.Position.y;
    float zDistance = cameraTransform.Position.z;
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
            hitPosition = glm::ivec3(static_cast<int>(floor(xDistance)) - (!right && xyzChoice == 0 ? 1 : 0), static_cast<int>(floor(yDistance)) - (!up && xyzChoice == 1 ? 1 : 0), static_cast<int>(floor(zDistance)) - (!forward && xyzChoice == 2 ? 1 : 0));
            if (World::GetWorld()->IsBlockExists(hitPosition.x, hitPosition.y, hitPosition.z))
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

float Player::CalculateMaxDistanceForHighlight(const glm::vec3& front, const bool up, const bool right, const bool forward) const
{
    float xDistance = right ? 4.0F + EngineDefaults::PLAYER_SIZE.x : 3.0F + EngineDefaults::PLAYER_SIZE.x;
    float yDistance = up ? 4.0F + 2 * EngineDefaults::PLAYER_SIZE.y - EngineDefaults::CAMERA_OFFSET : 3.0F + EngineDefaults::CAMERA_OFFSET;
    float zDistance = forward ? 4.0F + EngineDefaults::PLAYER_SIZE.z : 3.0F + EngineDefaults::PLAYER_SIZE.z;
    xDistance /= front.x;
    yDistance /= front.y;
    zDistance /= front.z;
    xDistance = abs(xDistance);
    yDistance = abs(yDistance);
    zDistance = abs(zDistance);
    return glm::min(xDistance, glm::min(yDistance, zDistance));
}

void Player::PlaceBlock(const glm::ivec3& hitPos, const BlockFaces& faceHit) const
{
    const int x = hitPos.x;
    const int y = hitPos.y;
    const int z = hitPos.z;
    const Block* blockToPlace = BlockTypeList::GetBlockTypeData(State.GetState().CurrentSelectedBlock);
    BoundingBox box = blockToPlace->GetBoundingBox();
    switch (faceHit)
    {
    case BlockFaces::Bottom:
        box.Move(static_cast<float>(x), static_cast<float>(y - 1), static_cast<float>(z));
        if (blockToPlace->IsSolidBlock() && box.IsIntersecting(GetBoundingBox()))
        {
            break;
        }
        World::GetWorld()->PlaceBlockAt(x, y - 1, z, State.GetState().CurrentSelectedBlock);
        break;
    case BlockFaces::Top:
        box.Move(static_cast<float>(x), static_cast<float>(y + 1), static_cast<float>(z));
        if (blockToPlace->IsSolidBlock() && box.IsIntersecting(GetBoundingBox()))
        {
            break;
        }
        World::GetWorld()->PlaceBlockAt(x, y + 1, z, State.GetState().CurrentSelectedBlock);
        break;
    case BlockFaces::North:
        box.Move(static_cast<float>(x), static_cast<float>(y), static_cast<float>(z + 1));
        if (blockToPlace->IsSolidBlock() && box.IsIntersecting(GetBoundingBox()))
        {
            break;
        }
        World::GetWorld()->PlaceBlockAt(x, y, z + 1, State.GetState().CurrentSelectedBlock);
        break;
    case BlockFaces::South:
        box.Move(static_cast<float>(x), static_cast<float>(y), static_cast<float>(z - 1));
        if (blockToPlace->IsSolidBlock() && box.IsIntersecting(GetBoundingBox()))
        {
            break;
        }
        World::GetWorld()->PlaceBlockAt(x, y, z - 1, State.GetState().CurrentSelectedBlock);
        break;
    case BlockFaces::East:
        box.Move(static_cast<float>(x + 1), static_cast<float>(y), static_cast<float>(z));
        if (blockToPlace->IsSolidBlock() && box.IsIntersecting(GetBoundingBox()))
        {
            break;
        }
        World::GetWorld()->PlaceBlockAt(x + 1, y, z, State.GetState().CurrentSelectedBlock);
        break;
    case BlockFaces::West:
        box.Move(static_cast<float>(x - 1), static_cast<float>(y), static_cast<float>(z));
        if (blockToPlace->IsSolidBlock() && box.IsIntersecting(GetBoundingBox()))
        {
            break;
        }
        World::GetWorld()->PlaceBlockAt(x - 1, y, z, State.GetState().CurrentSelectedBlock);
        break;
    }
}

void Player::ApplyRevertEntityChange(const std::vector<uint8_t>& changes, size_t& pos, const EChangeTypeEntity change, bool revert)
{
    switch (change)
    {
    case EChangeTypeEntity::PlayerInputState:
        if (revert)
        {
            State.SetInputState(EngineDefaults::ReadDataFromVector<ClientInputStruct>(changes, pos));
            pos += sizeof(ClientInputStruct);
        }
        else
        {
            pos += sizeof(ClientInputStruct);
            State.SetInputState(EngineDefaults::ReadDataFromVector<ClientInputStruct>(changes, pos));
        }
        break;
    case EChangeTypeEntity::PlayerCameraPitch:
        if (revert)
        {
            State.SetCameraPitch(EngineDefaults::ReadDataFromVector<float>(changes, pos));
            pos += sizeof(float);
        }
        else
        {
            pos += sizeof(float);
            State.SetCameraPitch(EngineDefaults::ReadDataFromVector<float>(changes, pos));
        }
        break;
    default:
        LivingEntity::ApplyRevertEntityChange(changes, pos, change, revert);
        break;
    }
}

void Player::SetClientInput(const uint64_t index, const ClientInputStatusStruct& input)
{
    ClientInputs[index % ClientInputs.size()] = ClientInputs[(index - 1) % ClientInputs.size()];
    ClientInputs[index % ClientInputs.size()] = ClientInputs[index % ClientInputs.size()] << input;
}

void Player::HandleClientInput()
{
    const ClientInputStruct& input = State.GetState().InputState;
    int8_t verticalInput = 0;
    int8_t horizontalInput = 0;
    if (input.IsKeyHold(EKeySet::Up))
    {
        verticalInput += 1;
    }
    if (input.IsKeyHold(EKeySet::Down))
    {
        verticalInput -= 1;
    }
    if (input.IsKeyHold(EKeySet::Left))
    {
        horizontalInput -= 1;
    }
    if (input.IsKeyHold(EKeySet::Right))
    {
        horizontalInput += 1;
    }
    State.SetVerticalInput(verticalInput);
    State.SetHorizontalInput(horizontalInput);
    State.SetJumpRequested(input.IsKeyHold(EKeySet::Jump));
    State.SetCameraPitch(std::clamp<float>(State.GetState().CameraPitch - input.MouseY, -89.0F, 89.0F));
    const glm::vec3& rot = State.GetState().EntityTransform.Rotation;
    State.SetRotation(glm::vec3(rot.x, rot.y + input.MouseX, rot.z));
    if (input.IsKeyPressed(EKeySet::One))
    {
        State.SetCurrentSelectedBlock(EBlockType::Stone);
    }
    if (input.IsKeyPressed(EKeySet::Two))
    {
        State.SetCurrentSelectedBlock(EBlockType::Dirt);
    }
    if (input.IsKeyPressed(EKeySet::Three))
    {
        State.SetCurrentSelectedBlock(EBlockType::Cobblestone);
    }
    if (input.IsKeyPressed(EKeySet::Four))
    {
        State.SetCurrentSelectedBlock(EBlockType::Planks);
    }
    if (input.IsKeyPressed(EKeySet::Five))
    {
        State.SetCurrentSelectedBlock(EBlockType::Sapling);
    }
    if (input.IsKeyPressed(EKeySet::RightMouseButton))
    {
        State.SetMode(!State.GetState().Mode);
    }
    if (input.IsKeyPressed(EKeySet::Reset))
    {
        State.SetPosition(glm::vec3(World::GetWorld()->TickRandomEngine.GetNextFloat() * 256.0F, 67.0F, World::GetWorld()->TickRandomEngine.GetNextFloat() * 256.0F));
    }
    if (input.IsKeyPressed(EKeySet::LeftMouseButton))
    {
        bool found = false;
        glm::ivec3 hitPosition;
        const BlockFaces hitFace = FindClosestFace(found, hitPosition);
        if (found)
        {
            if (State.GetState().Mode)
            {
                PlaceBlock(hitPosition, hitFace);
            }
            else
            {
                World::GetWorld()->RemoveBlockAt(hitPosition.x, hitPosition.y, hitPosition.z);
            }
        }
    }
    World::GetWorld()->EntityChanged(State.GetState().EntityId);
    // Add more input handling
}


/*
void Player::HandleKeyChangePacket(const KeyChangePacket& packet)
{
    const int action = packet.GetAction();
    switch (packet.GetKey())
    {
    case GLFW_KEY_G:
    default:
        break;
    }
}

void Player::HandleMouseClickPacket(const MouseChangePacket& packet)
{
    if (packet.GetButton() == GLFW_MOUSE_BUTTON_LEFT)
    {
        if (packet.GetAction() == GLFW_PRESS)
        {
            if (Mode)
            {
                PlaceBlock();
            }
            else if (BlockHit != nullptr)
            {
                GetWorld()->RemoveBlockAt(BlockHitPosition.x, BlockHitPosition.y, BlockHitPosition.z);
            }
        }
    }
}
*/