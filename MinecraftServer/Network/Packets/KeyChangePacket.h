#pragma once
#include "Packet.h"
#include "PacketData.h"

class KeyChangePacket final : public PacketData
{
    int Key;
    int Action;

public:
    KeyChangePacket(std::shared_ptr<ConnectionToClient> connection, Packet& packetToRead);
    [[nodiscard]] int GetKey() const;
    [[nodiscard]] int GetAction() const;
    [[nodiscard]] EPacketType GetPacketType() const override;
};
