#pragma once
#include "PacketData.h"

class MouseChangePacket final : PacketData
{
    int Button;
    int Action;

public:
    MouseChangePacket(std::shared_ptr<ConnectionToClient> client, Packet& packetToRead);
    int GetButton() const;
    int GetAction() const;
};
