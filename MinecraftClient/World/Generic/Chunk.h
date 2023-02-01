#pragma once
#include "Blocks/BlockTypeList.h"
#include "Entities/Generic/Camera.h"
#include "Util/ChunkCoords.h"
#include "Util/States/ChunkState.h"
#include "Util/TessellationHelper.h"
#include <GLFW/glfw3.h>

class Chunk
{
    TessellationHelper Tessellation;
    ChunkState State;
    TransformStruct ChunkTransform{};
    double TimeGotDirty = 0.0;
    static World* TheWorld;
    void DrawBlock(EBlockType blockType, int x, int y, int z);
    void DrawSaplingBlock(const Block* block, int x, int y, int z);
    [[nodiscard]] bool IsSideNeedToBeDrawn(const BlockFaces& face, int x, int y, int z) const;
    void DrawDefaultBlock(const Block* block, int x, int y, int z);
    [[nodiscard]] float GetDistanceFromPlayer() const;

public:
    static void SetWorld(::World* newWorld);
    [[nodiscard]] const ChunkState& GetChunkState() const;
    Chunk(int x, int y, int z);
    explicit Chunk(const ChunkState& state);
    [[nodiscard]] const Block* GetBlockAt(int x, int y, int z) const;
    [[nodiscard]] EBlockType GetBlockTypeAt(int x, int y, int z) const;
    [[nodiscard]] EBlockType GetBlockTypeAt(uint16_t index) const;
    [[nodiscard]] bool IsCoordsInsideChunk(int x, int y, int z) const;
    void ResetDraw();
    void Draw();
    void GenerateTessellationData();
    void SetBlockTypeAt(int x, int y, int z, EBlockType block);
    void SetBlockTypeAt(uint16_t index, EBlockType block);
    bool operator<(const Chunk& other) const;
    [[nodiscard]] bool ChunkInFrustum(const Frustum& frustum) const;
    void GotDirty();
    [[nodiscard]] ChunkCoords GetChunkPos() const;
    void ApplyChunkChanges(const std::vector<uint8_t>& changes, size_t& pos);
    void RevertChunkState(const ChunkState& previousChunkState);
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