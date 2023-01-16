#pragma once
#include "Blocks/BlockTypeList.h"
#include "Entities/Generic/Camera.h"
#include "Util/TessellationHelper.h"
#include <GLFW/glfw3.h>

template <typename T>
struct ChunkCoords;

class Block;

class EngineDefaults;

class CustomFileManager;

template <typename T>
class Chunk
{
    ChunkCoords<T> ChunkPosition;
    TessellationHelper Tessellation;
    double TimeGotDirty = 0.0;
    static T* World;
    void DrawBlock(EBlockType blockType, int x, int y, int z);
    void DrawSaplingBlock(const Block* block, int x, int y, int z);
    [[nodiscard]] bool IsSideNeedToBeDrawn(const BlockFaces& face, int x, int y, int z) const;
    void DrawDefaultBlock(const Block* block, int x, int y, int z);
    [[nodiscard]] float GetDistanceFromPlayer() const;

public:
    static constexpr char CHUNK_WIDTH = 16;
    static constexpr char CHUNK_HEIGHT = 16;
    static constexpr char CHUNK_DEPTH = 16;
    static void SetWorld(T* newWorld);
    static T* GetWorld();
    Chunk(int x, int y, int z);
    [[nodiscard]] const Block* GetBlockAt(int x, int y, int z) const;
    [[nodiscard]] EBlockType GetBlockTypeAt(int x, int y, int z) const;
    [[nodiscard]] bool IsCoordsInsideChunk(int x, int y, int z) const;
    void ResetDraw();
    void Draw();
    void GenerateTessellationData();
    void SetBlockTypeAt(int x, int y, int z, EBlockType block);
    bool operator<(const Chunk& other) const;
    [[nodiscard]] bool ChunkInFrustum(const Frustum& frustum) const;
    void GotDirty();
    ChunkCoords<T> GetChunkPos();
    std::array<EBlockType, static_cast<size_t>(Chunk<T>::CHUNK_HEIGHT * Chunk<T>::CHUNK_DEPTH * Chunk<T>::CHUNK_WIDTH)>& GetBlocks();

private:
    std::array<EBlockType, static_cast<size_t>(CHUNK_HEIGHT * CHUNK_DEPTH * CHUNK_WIDTH)> Blocks{EBlockType::Air};
};

template <typename T>
struct DirtyChunkComparator
{
    Frustum CameraFrustum;
    explicit DirtyChunkComparator(const Frustum& cameraFrustum);

    bool operator()(const Chunk<T>* a, const Chunk<T>* b) const
    {
        if (const bool isInFrustum = a->ChunkInFrustum(CameraFrustum); isInFrustum != b->ChunkInFrustum(CameraFrustum))
        {
            return isInFrustum;
        }
        return *a < *b;
    }
};

template <typename T>
T* Chunk<T>::World = nullptr;

template <typename T>
void Chunk<T>::SetWorld(T* newWorld)
{
    World = newWorld;
}

template <typename T>
T* Chunk<T>::GetWorld()
{
    return World;
}

template <typename T>
Chunk<T>::Chunk(const int x, const int y, const int z) : ChunkPosition(x, y, z)
{
    GetWorld()->AddChunkAsDirty(this);
    for (auto& block : Blocks)
    {
        block = EBlockType::Air;
    }
}

template <typename T>
void Chunk<T>::DrawBlock(const EBlockType blockType, const int x, const int y, const int z)
{
    const Block* block = BlockTypeList::GetBlockTypeData(blockType);
    const int finalX = x + ChunkPosition.X * CHUNK_WIDTH;
    const int finalY = y + ChunkPosition.Y * CHUNK_HEIGHT;
    const int finalZ = z + ChunkPosition.Z * CHUNK_DEPTH;
    switch (block->GetDrawType())
    {
    case DrawType::Default:
        DrawDefaultBlock(block, finalX, finalY, finalZ);
        break;
    case DrawType::Sapling:
        DrawSaplingBlock(block, finalX, finalY, finalZ);
        break;
    }
}

template <typename T>
void Chunk<T>::DrawSaplingBlock(const Block* block, const int x, int y, const int z)
{
    if (IsSideNeedToBeDrawn(BlockFaces::Top, x, y, z) || IsSideNeedToBeDrawn(BlockFaces::Bottom, x, y, z) || IsSideNeedToBeDrawn(BlockFaces::North, x, y, z) || IsSideNeedToBeDrawn(BlockFaces::South, x, y, z) || IsSideNeedToBeDrawn(BlockFaces::East, x, y, z) || IsSideNeedToBeDrawn(BlockFaces::West, x, y, z))
    {
        const int brightness = World->GetBrightnessAt(x, y + 1, z);
        block->GenerateTessellationData(Tessellation, BlockFaces::Top, static_cast<float>(x), static_cast<float>(y), static_cast<float>(z), brightness);
    }
}

template <typename T>
bool Chunk<T>::IsSideNeedToBeDrawn(const BlockFaces& face, const int x, const int y, const int z) const
{
    switch (face)
    {
    case BlockFaces::Top:
        {
            if (IsCoordsInsideChunk(x, y + 1, z))
            {
                return !BlockTypeList::GetBlockTypeData(Blocks[static_cast<size_t>(EngineDefaults::GetChunkLocalIndex<T>(x, y + 1, z))])->IsSolidBlock();
            }
            return !GetWorld()->IsBlockSolid(x, y + 1, z);
        }
    case BlockFaces::Bottom:
        if (IsCoordsInsideChunk(x, y - 1, z))
        {
            return !BlockTypeList::GetBlockTypeData(Blocks[static_cast<size_t>(EngineDefaults::GetChunkLocalIndex<T>(x, y - 1, z))])->IsSolidBlock();
        }
        return !GetWorld()->IsBlockSolid(x, y - 1, z);
    case BlockFaces::North:
        if (IsCoordsInsideChunk(x, y, z + 1))
        {
            return !BlockTypeList::GetBlockTypeData(Blocks[static_cast<size_t>(EngineDefaults::GetChunkLocalIndex<T>(x, y, z + 1))])->IsSolidBlock();
        }
        return !GetWorld()->IsBlockSolid(x, y, z + 1);
    case BlockFaces::South:
        if (IsCoordsInsideChunk(x, y, z - 1))
        {
            return !BlockTypeList::GetBlockTypeData(Blocks[static_cast<size_t>(EngineDefaults::GetChunkLocalIndex<T>(x, y, z - 1))])->IsSolidBlock();
        }
        return !GetWorld()->IsBlockSolid(x, y, z - 1);
    case BlockFaces::East:
        if (IsCoordsInsideChunk(x + 1, y, z))
        {
            return !BlockTypeList::GetBlockTypeData(Blocks[static_cast<size_t>(EngineDefaults::GetChunkLocalIndex<T>(x + 1, y, z))])->IsSolidBlock();
        }
        return !GetWorld()->IsBlockSolid(x + 1, y, z);
    case BlockFaces::West:
        if (IsCoordsInsideChunk(x - 1, y, z))
        {
            return !BlockTypeList::GetBlockTypeData(Blocks[static_cast<size_t>(EngineDefaults::GetChunkLocalIndex<T>(x - 1, y, z))])->IsSolidBlock();
        }
        return !GetWorld()->IsBlockSolid(x - 1, y, z);
    }
    return false;
}

template <typename T>
void Chunk<T>::DrawDefaultBlock(const Block* block, const int x, const int y, const int z)
{
    for (int face = static_cast<int>(BlockFaces::Top); face <= static_cast<int>(BlockFaces::South); face++)
    {
        int brightness;
        switch (static_cast<BlockFaces>(face))
        {
        case BlockFaces::Bottom:
            brightness = World->GetBrightnessAt(x, y - 1, z);
            break;
        case BlockFaces::North:
            brightness = World->GetBrightnessAt(x, y, z + 1);
            break;
        case BlockFaces::South:
            brightness = World->GetBrightnessAt(x, y, z - 1);
            break;
        case BlockFaces::East:
            brightness = World->GetBrightnessAt(x + 1, y, z);
            break;
        case BlockFaces::West:
            brightness = World->GetBrightnessAt(x - 1, y, z);
            break;
        case BlockFaces::Top:
            brightness = World->GetBrightnessAt(x, y + 1, z);
            break;
        }
        if (IsSideNeedToBeDrawn(static_cast<BlockFaces>(face), x, y, z))
        {
            block->GenerateTessellationData(Tessellation, static_cast<BlockFaces>(face), static_cast<float>(x), static_cast<float>(y), static_cast<float>(z), brightness);
        }
    }
}

template <typename T>
float Chunk<T>::GetDistanceFromPlayer() const
{
    const auto* player = GetWorld()->GetPlayer();
    if (player == nullptr)
    {
        return 0.0F;
    }
    const glm::vec3 temp = player->GetTransform().GetPosition() - glm::vec3(static_cast<float>(ChunkPosition.X * CHUNK_WIDTH) + CHUNK_WIDTH / 2.0F, static_cast<float>(ChunkPosition.Y * CHUNK_HEIGHT) + CHUNK_HEIGHT / 2.0F, static_cast<float>(ChunkPosition.Z * CHUNK_DEPTH) + CHUNK_DEPTH / 2.0F);
    return dot(temp, temp);
}

template <typename T>
DirtyChunkComparator<T>::DirtyChunkComparator(const Frustum& cameraFrustum) : CameraFrustum(cameraFrustum)
{
}

template <typename T>
const Block* Chunk<T>::GetBlockAt(const int x, const int y, const int z) const
{
    return BlockTypeList::GetBlockTypeData(GetBlockTypeAt(x, y, z));
}

template <typename T>
EBlockType Chunk<T>::GetBlockTypeAt(const int x, const int y, const int z) const
{
    return Blocks[static_cast<size_t>(EngineDefaults::GetChunkLocalIndex<T>(x, y, z))];
}

template <typename T>
bool Chunk<T>::IsCoordsInsideChunk(const int x, const int y, const int z) const
{
    return x >= ChunkPosition.X && y >= ChunkPosition.Y && z >= ChunkPosition.Z && x < ChunkPosition.X + CHUNK_WIDTH && y < ChunkPosition.Y + CHUNK_HEIGHT && z < ChunkPosition.Z + CHUNK_DEPTH;
}

template <typename T>
void Chunk<T>::ResetDraw()
{
    Tessellation.Reset();
}

template <typename T>
void Chunk<T>::Draw()
{
    Tessellation.Draw();
    if (Tessellation.HasInitialized())
    {
        Tessellation.FreeMemory();
    }
}

template <typename T>
void Chunk<T>::GenerateTessellationData()
{
    if (Tessellation.HasInitialized())
    {
        ResetDraw();
    }
    for (int x = 0; x < CHUNK_WIDTH; x++)
    {
        for (int y = 0; y < CHUNK_HEIGHT; y++)
        {
            for (int z = 0; z < CHUNK_DEPTH; z++)
            {
                if (const EBlockType type = Blocks[static_cast<size_t>(EngineDefaults::GetChunkLocalIndex<T>(x, y, z))]; type != EBlockType::Air)
                {
                    DrawBlock(type, x, y, z);
                }
            }
        }
    }
}

template <typename T>
void Chunk<T>::SetBlockTypeAt(const int x, const int y, const int z, const EBlockType block)
{
    const auto index = static_cast<size_t>(EngineDefaults::GetChunkLocalIndex<T>(x, y, z));
    const EBlockType check = Blocks[index];
    Blocks[index] = block;
    if (check != block)
    {
        GetWorld()->AddChunkAsDirty(this);
    }
}

template <typename T>
bool Chunk<T>::operator<(const Chunk& other) const
{
    if (other.TimeGotDirty - TimeGotDirty > 2.0)
    {
        return true;
    }
    return GetDistanceFromPlayer() < other.GetDistanceFromPlayer();
}

template <typename T>
bool Chunk<T>::ChunkInFrustum(const Frustum& frustum) const
{
    return frustum.CubeInFrustum(static_cast<float>(ChunkPosition.X * CHUNK_WIDTH),
                                 static_cast<float>(ChunkPosition.Y * CHUNK_HEIGHT),
                                 static_cast<float>(ChunkPosition.Z * CHUNK_DEPTH),
                                 static_cast<float>(ChunkPosition.X * CHUNK_WIDTH + CHUNK_WIDTH),
                                 static_cast<float>(ChunkPosition.Y * CHUNK_HEIGHT + CHUNK_HEIGHT),
                                 static_cast<float>(ChunkPosition.Z * CHUNK_DEPTH + CHUNK_DEPTH));
}

template <typename T>
void Chunk<T>::GotDirty()
{
    TimeGotDirty = glfwGetTime();
}

template <typename T>
ChunkCoords<T> Chunk<T>::GetChunkPos()
{
    return ChunkPosition;
}

template <typename T>
std::array<EBlockType, static_cast<size_t>(Chunk<T>::CHUNK_HEIGHT * Chunk<T>::CHUNK_DEPTH * Chunk<T>::CHUNK_WIDTH)>&
Chunk<T>::GetBlocks()
{
    return Blocks;
}
