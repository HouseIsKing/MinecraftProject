#pragma once
#include "Blocks/Block.h"
#include "ChunkCoords.h"
#include <array>

#include "Network/ConnectionToClient.h"

class MultiPlayerWorld;

class Chunk
{
public:
    static constexpr char CHUNK_WIDTH = 16;
    static constexpr char CHUNK_HEIGHT = 16;
    static constexpr char CHUNK_DEPTH = 16;
    static void SetWorld(MultiPlayerWorld* newWorld);
    static MultiPlayerWorld* GetWorld();
    Chunk(int x, int y, int z);
    [[nodiscard]] const Block* GetBlockAt(int x, int y, int z) const;
    [[nodiscard]] EBlockType GetBlockTypeAt(int x, int y, int z) const;
    void SetBlockTypeAt(int x, int y, int z, EBlockType block);
    void SendChunkToClient(ConnectionToClient* client) const;
    friend CustomFileManager& operator<<(CustomFileManager& fileManager, const Chunk& chunk);
    friend CustomFileManager& operator>>(CustomFileManager& fileManager, Chunk& chunk);

private:
    ChunkCoords ChunkPosition;
    static MultiPlayerWorld* World;
    std::array<EBlockType, static_cast<size_t>(CHUNK_HEIGHT * CHUNK_DEPTH * CHUNK_WIDTH)> Blocks{EBlockType::Air};
};
