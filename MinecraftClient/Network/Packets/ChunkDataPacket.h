#pragma once
#include "PacketData.h"
#include "Util/ChunkCoords.h"
#include "World/Generic/Chunk.h"

class MultiPlayerWorld;

class ChunkDataPacket final : public PacketData
{
    int X{};
    int Y{};
    int Z{};
    std::array<uint8_t, static_cast<size_t>(Chunk<MultiPlayerWorld>::CHUNK_HEIGHT * Chunk<MultiPlayerWorld>::CHUNK_WIDTH * Chunk<MultiPlayerWorld>::CHUNK_DEPTH)> ChunkData
        {};

public:
    explicit ChunkDataPacket(Packet& packet);
    [[nodiscard]] const std::array<uint8_t, static_cast<size_t>(Chunk<MultiPlayerWorld>::CHUNK_HEIGHT * Chunk<MultiPlayerWorld>::CHUNK_WIDTH * Chunk<MultiPlayerWorld>::CHUNK_DEPTH)>& GetChunkData() const;
    [[nodiscard]] int GetX() const;
    [[nodiscard]] int GetY() const;
    [[nodiscard]] int GetZ() const;
    [[nodiscard]] EPacketType GetPacketType() const override;
};
