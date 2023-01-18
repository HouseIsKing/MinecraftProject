#pragma once
#include "ChunkDataPacket.h"
#include "LightDataPacket.h"
#include "PacketData.h"

class WorldDataPacket final : public PacketData
{
    std::vector<std::unique_ptr<ChunkDataPacket>> Chunks;
    std::vector<std::unique_ptr<LightDataPacket>> Lights;

public:
    explicit WorldDataPacket(Packet& packet);
    [[nodiscard]] const std::vector<std::unique_ptr<ChunkDataPacket>>& GetChunks() const;
    [[nodiscard]] const std::vector<std::unique_ptr<LightDataPacket>>& GetLights() const;
};
