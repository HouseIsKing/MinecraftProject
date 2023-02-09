#pragma once
#include "Blocks/Block.h"
#include "Util/States/ChunkStateWrapper.h"

class Chunk
{
public:
    Chunk(int x, int y, int z);
    explicit Chunk(const ChunkCoords& coords);
    explicit Chunk(const ChunkState& state);
    [[nodiscard]] const Block* GetBlockAt(int x, int y, int z) const;
    [[nodiscard]] EBlockType GetBlockTypeAt(int x, int y, int z) const;
    [[nodiscard]] EBlockType GetBlockTypeAt(uint16_t index) const;
    [[nodiscard]] const ChunkState& GetState() const;
    void SetBlockTypeAt(int x, int y, int z, EBlockType block);
    void SetBlockTypeAt(uint16_t index, EBlockType block);
    void RevertChunkChanges(const std::vector<uint8_t>& changes, size_t& pos);
    void AttachChunkChanges(std::vector<uint8_t>& changes) const;
    void AttachChunkData(std::vector<uint8_t>& data) const;
    void ClearAllChanges();

protected:
    ChunkStateWrapper State;
};

