#include "Player.h"

#include <GLFW/glfw3.h>
#include "Util/EngineDefaults.h"
//#include "Entities/Zombie.h"
#include "World/MultiPlayerWorld.h"

Player::Player(const float x, const float y, const float z, ConnectionToClient* client) : LivingEntity(PLAYER_SIZE, x, y, z), Client(client), LeftMousePressed(false), RightMousePressed(false), PrevMouseX(0), PrevMouseY(0), IsSpawnZombieButtonPressed(false), FaceHit(BlockFaces::Top), BlockHit(nullptr), BlockHitPosition(), CurrentSelectedBlock(EBlockType::Stone)
{
    auto packet = Packet(PacketHeader::PLAYER_ID_PACKET);
    packet << GetEntityId();
    Client->WritePacket(packet);
    packet = Packet(PacketHeader::PLAYER_POSITION_PACKET);
    packet << GetEntityId() << x << y << z;
    Client->WritePacket(packet);
}

void Player::Tick()
{
    LivingEntity::Tick();
    glm::vec3 pos = GetTransform().GetPosition();
    pos.y += CAMERA_OFFSET - PLAYER_SIZE.y;
    CameraTransform.SetPosition(pos);
    bool found = false;
    FaceHit = FindClosestFace(found);
    BlockHit = found ? GetWorld()->GetBlockAt(BlockHitPosition.x, BlockHitPosition.y, BlockHitPosition.z) : nullptr;
    auto positionPacket = Packet(PacketHeader::PLAYER_POSITION_PACKET);
    pos = GetTransform().GetPosition();
    positionPacket << GetEntityId() << pos.x << pos.y << pos.z;
    Client->WritePacket(positionPacket);
    const glm::vec3 rotation = CameraTransform.GetRotation();
    auto rotationPacket = Packet(PacketHeader::PLAYER_ROTATION_PACKET);
    rotationPacket << GetEntityId() << rotation.x << rotation.y << rotation.z;
    Client->WritePacket(rotationPacket);
}

BlockFaces Player::FindClosestFace(bool& foundBlock)
{
    const glm::vec3 frontVector = CameraTransform.GetForwardVector();
    const glm::vec3 cameraPos = CameraTransform.GetPosition();
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
            BlockHitPosition = glm::ivec3(static_cast<int>(floor(xDistance)) - (!right && xyzChoice == 0 ? 1 : 0), static_cast<int>(floor(yDistance)) - (!up && xyzChoice == 1 ? 1 : 0), static_cast<int>(floor(zDistance)) - (!forward && xyzChoice == 2 ? 1 : 0));
            if (GetWorld()->IsBlockExists(BlockHitPosition.x, BlockHitPosition.y, BlockHitPosition.z))
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

void Player::PlaceBlock()
{
    if (BlockHit != nullptr)
    {
        const int x = BlockHitPosition.x;
        const int y = BlockHitPosition.y;
        const int z = BlockHitPosition.z;
        const Block* blockToPlace = BlockTypeList::GetBlockTypeData(CurrentSelectedBlock);
        BoundingBox box = blockToPlace->GetBoundingBox();
        switch (FaceHit)
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

bool Player::GetMode() const
{
    return Mode;
}

EBlockType Player::GetCurrentSelectedBlock() const
{
    return CurrentSelectedBlock;
}

void Player::HandleKeyChangePacket(const KeyChangePacket& packet)
{
    const int action = packet.GetAction();
    switch (packet.GetKey())
    {
    case GLFW_KEY_W:
        if (action == GLFW_PRESS)
        {
            VerticalInput++;
        }
        else if (action == GLFW_RELEASE)
        {
            VerticalInput--;
        }
        break;
    case GLFW_KEY_S:
        if (action == GLFW_PRESS)
        {
            VerticalInput--;
        }
        else if (action == GLFW_RELEASE)
        {
            VerticalInput++;
        }
        break;
    case GLFW_KEY_A:
        if (action == GLFW_PRESS)
        {
            HorizontalInput--;
        }
        else if (action == GLFW_RELEASE)
        {
            HorizontalInput++;
        }
        break;
    case GLFW_KEY_D:
        if (action == GLFW_PRESS)
        {
            HorizontalInput++;
        }
        else if (action == GLFW_RELEASE)
        {
            HorizontalInput--;
        }
        break;
    case GLFW_KEY_SPACE:
        if (action == GLFW_PRESS)
        {
            JumpRequested = true;
        }
        else if (action == GLFW_RELEASE)
        {
            JumpRequested = false;
        }
        break;
    case GLFW_KEY_1:
        if (action == GLFW_PRESS)
        {
            CurrentSelectedBlock = EBlockType::Stone;
        }
        break;
    case GLFW_KEY_2:
        if (action == GLFW_PRESS)
        {
            CurrentSelectedBlock = EBlockType::Dirt;
        }
        break;
    case GLFW_KEY_3:
        if (action == GLFW_PRESS)
        {
            CurrentSelectedBlock = EBlockType::Cobblestone;
        }
        break;
    case GLFW_KEY_4:
        if (action == GLFW_PRESS)
        {
            CurrentSelectedBlock = EBlockType::Planks;
        }
        break;
    case GLFW_KEY_5:
        if (action == GLFW_PRESS)
        {
            CurrentSelectedBlock = EBlockType::Sapling;
        }
        break;
    case GLFW_KEY_R:
        GetTransform().SetPosition(EngineDefaults::GetNextFloat() * 256.0F, 67.0F, EngineDefaults::GetNextFloat() * 256.0F);
        break;
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
    else if (packet.GetButton() == GLFW_MOUSE_BUTTON_RIGHT)
    {
        if (packet.GetAction() == GLFW_PRESS)
        {
            Mode = !Mode;
        }
    }
}

void Player::HandleMouseMovementPacket(const MousePosChangePacket& packet)
{
    CameraTransform.Rotate(-packet.GetY(), packet.GetX(), 0.0F);
    const glm::vec3 rotation = CameraTransform.GetRotation();
    if (rotation.x > 89.0F)
    {
        CameraTransform.SetRotation(89.0F, rotation.y, rotation.z);
    }
    else if (rotation.x < -89.0F)
    {
        CameraTransform.SetRotation(-89.0F, rotation.y, rotation.z);
    }
    GetTransform().SetRotation(0.0F, rotation.y, 0.0F);
}