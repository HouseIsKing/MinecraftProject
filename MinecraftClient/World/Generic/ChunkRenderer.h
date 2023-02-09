#pragma once
#include "Blocks/BlockRenderer.h"
#include "Entities/Generic/Camera.h"
#include "Util/States/ChunkState.h"

class ChunkRenderer
{
    const ChunkState& State;
    TransformStruct ChunkTransform;
    TessellationHelper Tessellation;
    double TimeGotDirty = 0.0;
    void DrawBlock(EBlockType blockType, int x, int y, int z);
    void DrawSaplingBlock(const BlockRenderer* block, int x, int y, int z, EBlockType blockType);
    void DrawDefaultBlock(const BlockRenderer* block, int x, int y, int z, EBlockType blockType);
    [[nodiscard]] bool IsSideNeedToBeDrawn(const BlockFaces& face, int x, int y, int z) const;
    [[nodiscard]] float GetDistanceFromPlayer() const;

public:
    [[nodiscard]] static bool IsCoordsInsideChunk(int x, int y, int z);
    explicit ChunkRenderer(const ChunkState& state);
    void ResetDraw();
    void Draw();
    void GenerateTessellationData();
    bool operator<(const ChunkRenderer& other) const;
    [[nodiscard]] bool ChunkInFrustum(const Frustum& frustum) const;
    void GotDirty();
};

struct DirtyChunkComparator
{
    Frustum CameraFrustum;
    explicit DirtyChunkComparator(const Frustum& cameraFrustum);

    bool operator()(const ChunkRenderer* a, const ChunkRenderer* b) const
    {
        if (const bool isInFrustum = a->ChunkInFrustum(CameraFrustum); isInFrustum != b->ChunkInFrustum(CameraFrustum))
        {
            return isInFrustum;
        }
        return *a < *b;
    }
};