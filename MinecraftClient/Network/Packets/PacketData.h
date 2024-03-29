#pragma once
#include "Util/EngineDefaults.h"
#include <memory>

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
    [[nodiscard]] virtual EPacketType GetPacketType() const = 0;
};
