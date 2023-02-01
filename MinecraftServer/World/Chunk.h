#pragma once
#include "Blocks/Block.h"
#include "ChunkCoords.h"
#include "Network/ConnectionToClient.h"
#include "Util/States/ChunkState.h"
#include <array>

class MultiPlayerWorld;

class Chunk
{
public:
    static void SetWorld(MultiPlayerWorld* newWorld);
    static MultiPlayerWorld* GetWorld();
    Chunk(int x, int y, int z);
    explicit Chunk(const ChunkState& state);
    [[nodiscard]] const Block* GetBlockAt(int x, int y, int z) const;
    [[nodiscard]] EBlockType GetBlockTypeAt(int x, int y, int z) const;
    [[nodiscard]] const ChunkState& GetChunkState() const;
    void SetBlockTypeAt(int x, int y, int z, EBlockType block);
    void ApplyChunkChanges(const std::vector<uint8_t>& changes, size_t& pos);
    void RevertChunkState(const ChunkState& previousChunkState);
    void SendChunkToClient(const std::shared_ptr<Packet>& packet) const;
    friend CustomFileManager& operator<<(CustomFileManager& fileManager, const Chunk& chunk);
    friend CustomFileManager& operator>>(CustomFileManager& fileManager, Chunk& chunk);

private:
    ChunkState State;
    static MultiPlayerWorld* World;
};
