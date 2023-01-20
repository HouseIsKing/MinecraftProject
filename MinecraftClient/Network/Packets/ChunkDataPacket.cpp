#include "ChunkDataPacket.h"

ChunkDataPacket::ChunkDataPacket(Packet& packet)
{
    packet >> X >> Y >> Z;
    for (unsigned char& i : ChunkData)
    {
        packet >> i;
    }
}

const std::array<uint8_t, static_cast<size_t>(Chunk<MultiPlayerWorld>::CHUNK_HEIGHT * Chunk<MultiPlayerWorld>::CHUNK_WIDTH *
                     Chunk<MultiPlayerWorld>::CHUNK_DEPTH)>& ChunkDataPacket::GetChunkData() const
{
    return ChunkData;
}

int ChunkDataPacket::GetX() const
{
    return X;
}

int ChunkDataPacket::GetY() const
{
    return Y;
}

int ChunkDataPacket::GetZ() const
{
    return Z;
}

EPacketType ChunkDataPacket::GetPacketType() const
{
    return EPacketType::ChunkData;
}
