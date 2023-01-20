#pragma once
#include "Packet.h"
#include "PacketData.h"

class MouseChangePacket final : public PacketData
{
    int Button;
    int Action;

public:
    MouseChangePacket(std::shared_ptr<ConnectionToClient> client, Packet& packetToRead);
    [[nodiscard]] int GetButton() const;
    [[nodiscard]] int GetAction() const;
    [[nodiscard]] EPacketType GetPacketType() const override;
};
