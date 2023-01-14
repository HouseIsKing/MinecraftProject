#pragma once
#include "PacketData.h"

class KeyChangePacket final : PacketData
{
    int Key;
    int Action;

public:
    KeyChangePacket(std::shared_ptr<ConnectionToClient> connection, Packet& packetToRead);
    int GetKey() const;
    int GetAction() const;
};
