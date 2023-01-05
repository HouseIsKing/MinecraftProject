#pragma once
#include "../Entities/Player/Camera.h"
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
    void ResetDraw();
    void Draw();
    void GenerateTessellationData();
    void SetBlockTypeAt(int x, int y, int z, EBlockType block);
    friend CustomFileManager& operator<<(CustomFileManager& fileManager, const Chunk& chunk);
    friend CustomFileManager& operator>>(CustomFileManager& fileManager, Chunk& chunk);
    bool operator<(const Chunk& other) const;
    [[nodiscard]] bool ChunkInFrustum(const Frustum& frustum) const;
    void GotDirty();

private:
    ChunkCoords ChunkPosition;
    TessellationHelper Tessellation;
    double TimeGotDirty = 0.0;
    static SinglePlayerWorld* World;
    std::array<EBlockType, static_cast<size_t>(CHUNK_HEIGHT * CHUNK_DEPTH * CHUNK_WIDTH)> Blocks{EBlockType::Air};
    void DrawBlock(EBlockType blockType, int x, int y, int z);
    [[nodiscard]] float GetDistanceFromPlayer() const;
};

struct DirtyChunkComparator
{
    Frustum CameraFrustum;
    explicit DirtyChunkComparator(const Frustum& cameraFrustum);

    bool operator()(const Chunk* a, const Chunk* b) const
    {
        if (const bool isInFrustum = a->ChunkInFrustum(CameraFrustum); isInFrustum != b->ChunkInFrustum(CameraFrustum))
        {
            return isInFrustum;
        }
        return *a < *b;
    }
};
