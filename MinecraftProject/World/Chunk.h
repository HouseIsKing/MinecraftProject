//
// Created by amit on 4/21/2022.
//

#pragma once
#include "../Util/TessellationHelper.h"
#include "ChunkCoords.h"
#include <array>

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
    bool IsDirty;
    void ResetDraw() const;
    void Draw() const;
    void GenerateTessellationData();
    void SetBlockTypeAt(int x, int y, int z, EBlockType block);
    friend CustomFileManager& operator<<(CustomFileManager& fileManager, const Chunk& chunk);
    friend CustomFileManager& operator>>(CustomFileManager& fileManager, Chunk& chunk);
private:
    ChunkCoords ChunkPosition;
    unique_ptr<TessellationHelper> Tessellation;
    static SinglePlayerWorld* World;
    bool IsDirtyLights;
    std::array<EBlockType, static_cast<size_t>(CHUNK_HEIGHT * CHUNK_DEPTH * CHUNK_WIDTH)> Blocks{EBlockType::Air};
    void DrawBlock(EBlockType blockType, int x, int y, int z) const;
};
