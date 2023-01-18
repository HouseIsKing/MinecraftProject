#pragma once
#include "Entities/Generic/Camera.h"
#include "Entities/Generic/LivingEntity.h"
#include "Entities/Generic/PlayerSelectionHighlight.h"
#include "GUI/SelectedBlockGui.h"
#include "Network/ClientNetworkManager.h"
#include "Network/Packets/EntityDataPacket.h"
#include "Network/Packets/PlayerRotateChangePacket.h"
#include "World/Generic/Blocks/BlockTypeList.h"

class MultiPlayerWorld;

class PlayerMp final : public LivingEntity<MultiPlayerWorld>
{
    Camera& MyCamera;
    constexpr static glm::vec3 PLAYER_SIZE = glm::vec3(0.3F, 0.9F, 0.3F);
    constexpr static float CAMERA_OFFSET = 1.62F;
    bool LeftMousePressed;
    bool RightMousePressed;
    float PrevMouseX;
    float PrevMouseY;
    bool FirstMouseCheck = true;
    float MouseSensitivity = 0.15F;
    bool IsSpawnZombieButtonPressed;
    bool Mode = false;
    EBlockType CurrentSelectedBlock;
    SelectedBlockGui<MultiPlayerWorld>* SelectedBlockGuiPtr;
    PlayerSelectionHighlight<PlayerMp> SelectionHighlight;
    static int GetSelectionHighlightBrightness(int x, int y, int z, BlockFaces face);
    BlockFaces FindClosestFace(glm::ivec3& blockPosition, bool& foundBlock) const;
    [[nodiscard]] float CalculateMaxDistanceForHighlight(const glm::vec3& front, bool up, bool right, bool forward) const;
    void PlaceBlock() const;

public:
    void DisplaySelectionHighlight();
    explicit PlayerMp(const EntityDataPacket& data);
    void Render(float partialTick) override;
    [[nodiscard]] Frustum GetCameraFrustum() const;
    [[nodiscard]] float GetCameraPitch() const;
    [[nodiscard]] bool GetMode() const;
    [[nodiscard]] EBlockType GetCurrentSelectedBlock() const;
    void HandleEntityUpdate(const EntityDataPacket& packet) override;
    void HandleMouseMovementInput(float x, float y, ClientNetworkManager* network);
    void HandlePlayerRotationChange(const PlayerRotateChangePacket& packet) const;
};
