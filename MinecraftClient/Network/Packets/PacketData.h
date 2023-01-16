#pragma once
#include <memory>

#include "Packet.h"

class PacketData
{
protected:
    PacketData() = default;
    virtual ~PacketData() = default;

public:
    PacketData(const PacketData&) = delete;
    PacketData& operator=(const PacketData&) = delete;
    PacketData(PacketData&&) = delete;
    PacketData& operator=(PacketData&&) = delete;
};
