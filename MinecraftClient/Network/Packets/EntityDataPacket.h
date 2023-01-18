#pragma once
#include "PacketData.h"

class EntityDataPacket final : public PacketData
{
    uint16_t Id{};
    float XPos{};
    float YPos{};
    float ZPos{};
    float XRot{};
    float YRot{};
    float ZRot{};

public:
    explicit EntityDataPacket(Packet& packet);
    [[nodiscard]] uint16_t GetId() const;
    [[nodiscard]] float GetXPos() const;
    [[nodiscard]] float GetYPos() const;
    [[nodiscard]] float GetZPos() const;
    [[nodiscard]] float GetXRot() const;
    [[nodiscard]] float GetYRot() const;
    [[nodiscard]] float GetZRot() const;
};
