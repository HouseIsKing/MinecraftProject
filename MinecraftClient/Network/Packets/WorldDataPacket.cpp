#include "WorldDataPacket.h"

WorldDataPacket::WorldDataPacket(Packet& packet)
{
    uint32_t chunksCount;
    uint32_t lightsCount;
    packet >> chunksCount >> lightsCount;
    Chunks.reserve(chunksCount * sizeof(ChunkDataPacket));
    Lights.reserve(lightsCount * sizeof(LightDataPacket));
    for (uint32_t i = 0; i < chunksCount; i++)
    {
        Chunks.emplace_back(new ChunkDataPacket(packet));
    }
    for (uint32_t i = 0; i < lightsCount; i++)
    {
        Lights.emplace_back(new LightDataPacket(packet));
    }
}

const std::vector<std::unique_ptr<ChunkDataPacket>>& WorldDataPacket::GetChunks() const
{
    return Chunks;
}

const std::vector<std::unique_ptr<LightDataPacket>>& WorldDataPacket::GetLights() const
{
    return Lights;
}

EPacketType WorldDataPacket::GetPacketType() const
{
    return EPacketType::WorldData;
}
