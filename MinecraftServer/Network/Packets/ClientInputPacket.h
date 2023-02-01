#pragma once
#include "Packet.h"
#include "PacketData.h"

class ClientInputPacket final : public PacketData
{
    ClientInputState State;
    uint64_t WorldTickSent{};

public:
    ClientInputPacket(Packet& packet, std::shared_ptr<ConnectionToClient> connection);
    [[nodiscard]] const ClientInputState& GetState() const;
    [[nodiscard]] uint64_t GetWorldTickSent() const;
    [[nodiscard]] EPacketType GetPacketType() const override;
};
