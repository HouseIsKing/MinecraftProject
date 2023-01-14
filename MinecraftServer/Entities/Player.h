#pragma once
#include "LivingEntity.h"
#include "Network/ConnectionToClient.h"
#include "Network/Packets/KeyChangePacket.h"
#include "Network/Packets/MouseChangePacket.h"
#include "Network/Packets/MousePosChangePacket.h"
#include "World/Blocks/Block.h"
#include "World/Blocks/BlockTypeList.h"

class Player final : public LivingEntity
{
    constexpr static glm::vec3 PLAYER_SIZE = glm::vec3(0.3F, 0.9F, 0.3F);
    constexpr static float CAMERA_OFFSET = 1.62F;
    ConnectionToClient* Client;
    std::string Name;
    bool LeftMousePressed;
    bool RightMousePressed;
    float PrevMouseX;
    float PrevMouseY;
    Transform CameraTransform;
    float MouseSensitivity = 0.15F;
    bool IsSpawnZombieButtonPressed;
    BlockFaces FaceHit;
    const Block* BlockHit;
    glm::ivec3 BlockHitPosition;
    bool Mode = false;
    EBlockType CurrentSelectedBlock;
    BlockFaces FindClosestFace(bool& foundBlock);
    [[nodiscard]] float CalculateMaxDistanceForHighlight(const glm::vec3& front, bool up, bool right, bool forward) const;
    void PlaceBlock();

public:
    Player(float x, float y, float z, ConnectionToClient* client);
    void Tick() override;
    [[nodiscard]] bool GetMode() const;
    [[nodiscard]] EBlockType GetCurrentSelectedBlock() const;
    void HandleKeyChangePacket(const KeyChangePacket& packet);
    void HandleMouseClickPacket(const MouseChangePacket& packet);
    void HandleMouseMovementPacket(const MousePosChangePacket& packet);
};
