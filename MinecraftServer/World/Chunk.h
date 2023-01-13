#pragma once
#include "ChunkCoords.h"
#include <array>

#include "Blocks/Block.h"

class SinglePlayerWorld;

using std::array;
using std::unique_ptr;

class Chunk
{
public:
    static constexpr char CHUNK_WIDTH = 16;
    static constexpr char CHUNK_HEIGHT = 16;
    static constexpr char CHUNK_DEPTH = 16;
    static void SetWorld(SinglePlayerWorld* newWorld);
    static SinglePlayerWorld* GetWorld();
    Chunk(int x, int y, int z);
    [[nodiscard]] const Block* GetBlockAt(int x, int y, int z) const;
    [[nodiscard]] EBlockType GetBlockTypeAt(int x, int y, int z) const;
    [[nodiscard]] bool IsCoordsInsideChunk(int x, int y, int z) const;
    void SetBlockTypeAt(int x, int y, int z, EBlockType block);
    friend CustomFileManager& operator<<(CustomFileManager& fileManager, const Chunk& chunk);
    friend CustomFileManager& operator>>(CustomFileManager& fileManager, Chunk& chunk);
    bool operator<(const Chunk& other) const;

private:
    ChunkCoords ChunkPosition;
    double TimeGotDirty = 0.0;
    static SinglePlayerWorld* World;
    std::array<EBlockType, static_cast<size_t>(CHUNK_HEIGHT * CHUNK_DEPTH * CHUNK_WIDTH)> Blocks{EBlockType::Air};
    void DrawBlock(EBlockType blockType, int x, int y, int z);
    void DrawSaplingBlock(const Block* block, int x, int y, int z);
    [[nodiscard]] bool IsSideNeedToBeDrawn(const BlockFaces& face, int x, int y, int z) const;
    void DrawDefaultBlock(const Block* block, int x, int y, int z);
    [[nodiscard]] float GetDistanceFromPlayer() const;
};
