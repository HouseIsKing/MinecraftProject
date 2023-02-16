#pragma once
#include "Network/Packets/Packet.h"
#include "PacketData.h"

class ClientInputPacket final : public PacketData
{
    ClientInputStatusStruct State;
    uint64_t WorldTickSent{};

public:
    ClientInputPacket(Packet& packet, std::shared_ptr<ConnectionToClient> connection);
    [[nodiscard]] const ClientInputStatusStruct& GetState() const;
    [[nodiscard]] uint64_t GetWorldTickSent() const;
    [[nodiscard]] EPacketType GetPacketType() const override;
};
