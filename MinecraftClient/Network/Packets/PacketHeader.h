#pragma once
#include <array>
#include <cstdint>

#include "Util/EngineDefaults.h"
#include "Util/States/ClientInputState.h"

struct PacketHeader
{
    EPacketType PacketType;
    uint32_t PacketSize;
    explicit PacketHeader(EPacketType packetType);
    PacketHeader(EPacketType packetType, uint32_t packetSize);
    [[nodiscard]] std::array<uint8_t, 8> Serialize() const;
    static const PacketHeader CLIENT_INPUT_HEADER;
};

inline const PacketHeader PacketHeader::CLIENT_INPUT_HEADER = PacketHeader(EPacketType::ClientInput, sizeof(ClientInputState) + sizeof(uint64_t));
