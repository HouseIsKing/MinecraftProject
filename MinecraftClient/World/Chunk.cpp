#include "Chunk.h"
#include "Util/EngineDefaults.h"
#include "SinglePlayerWorld.h"

SinglePlayerWorld* Chunk::World = nullptr;

void Chunk::SetWorld(SinglePlayerWorld* newWorld)
{
    World = newWorld;
}

SinglePlayerWorld* Chunk::GetWorld()
{
    return World;
}

Chunk::Chunk(const int x, const int y, const int z) : ChunkPosition(x, y, z)
{
    GetWorld()->AddChunkAsDirty(this);
    for (auto& block : Blocks)
    {
        block = EBlockType::Air;
    }
}

void Chunk::DrawBlock(const EBlockType blockType, const int x, const int y, const int z)
{
    const Block* block = BlockTypeList::GetBlockTypeData(blockType);
    const int finalX = x + ChunkPosition.GetX() * CHUNK_WIDTH;
    const int finalY = y + ChunkPosition.GetY() * CHUNK_HEIGHT;
    const int finalZ = z + ChunkPosition.GetZ() * CHUNK_DEPTH;
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

void Chunk::DrawSaplingBlock(const Block* block, const int x, int y, const int z)
{
    if (IsSideNeedToBeDrawn(BlockFaces::Top, x, y, z) || IsSideNeedToBeDrawn(BlockFaces::Bottom, x, y, z) || IsSideNeedToBeDrawn(BlockFaces::North, x, y, z) || IsSideNeedToBeDrawn(BlockFaces::South, x, y, z) || IsSideNeedToBeDrawn(BlockFaces::East, x, y, z) || IsSideNeedToBeDrawn(BlockFaces::West, x, y, z))
    {
        const int brightness = World->GetBrightnessAt(x, y + 1, z);
        block->GenerateTessellationData(Tessellation, BlockFaces::Top, static_cast<float>(x), static_cast<float>(y), static_cast<float>(z), brightness);
    }
}

bool Chunk::IsSideNeedToBeDrawn(const BlockFaces& face, const int x, const int y, const int z) const
{
    switch (face)
    {
    case BlockFaces::Top:
        {
            if (IsCoordsInsideChunk(x, y + 1, z))
            {
                return !BlockTypeList::GetBlockTypeData(Blocks[static_cast<size_t>(EngineDefaults::GetChunkLocalIndex(x, y + 1, z))])->IsSolidBlock();
            }
            return !GetWorld()->IsBlockSolid(x, y + 1, z);
        }
    case BlockFaces::Bottom:
        if (IsCoordsInsideChunk(x, y - 1, z))
        {
            return !BlockTypeList::GetBlockTypeData(Blocks[static_cast<size_t>(EngineDefaults::GetChunkLocalIndex(x, y - 1, z))])->IsSolidBlock();
        }
        return !GetWorld()->IsBlockSolid(x, y - 1, z);
    case BlockFaces::North:
        if (IsCoordsInsideChunk(x, y, z + 1))
        {
            return !BlockTypeList::GetBlockTypeData(Blocks[static_cast<size_t>(EngineDefaults::GetChunkLocalIndex(x, y, z + 1))])->IsSolidBlock();
        }
        return !GetWorld()->IsBlockSolid(x, y, z + 1);
    case BlockFaces::South:
        if (IsCoordsInsideChunk(x, y, z - 1))
        {
            return !BlockTypeList::GetBlockTypeData(Blocks[static_cast<size_t>(EngineDefaults::GetChunkLocalIndex(x, y, z - 1))])->IsSolidBlock();
        }
        return !GetWorld()->IsBlockSolid(x, y, z - 1);
    case BlockFaces::East:
        if (IsCoordsInsideChunk(x + 1, y, z))
        {
            return !BlockTypeList::GetBlockTypeData(Blocks[static_cast<size_t>(EngineDefaults::GetChunkLocalIndex(x + 1, y, z))])->IsSolidBlock();
        }
        return !GetWorld()->IsBlockSolid(x + 1, y, z);
    case BlockFaces::West:
        if (IsCoordsInsideChunk(x - 1, y, z))
        {
            return !BlockTypeList::GetBlockTypeData(Blocks[static_cast<size_t>(EngineDefaults::GetChunkLocalIndex(x - 1, y, z))])->IsSolidBlock();
        }
        return !GetWorld()->IsBlockSolid(x - 1, y, z);
    }
    return false;
}

void Chunk::DrawDefaultBlock(const Block* block, const int x, const int y, const int z)
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

float Chunk::GetDistanceFromPlayer() const
{
    const PlayerController* player = GetWorld()->GetPlayer();
    if (player == nullptr)
    {
        return 0.0F;
    }
    const glm::vec3 temp = player->GetTransform().GetPosition() - glm::vec3(static_cast<float>(ChunkPosition.GetX() * CHUNK_WIDTH) + CHUNK_WIDTH / 2.0F, static_cast<float>(ChunkPosition.GetY() * CHUNK_HEIGHT) + CHUNK_HEIGHT / 2.0F, static_cast<float>(ChunkPosition.GetZ() * CHUNK_DEPTH) + CHUNK_DEPTH / 2.0F);
    return dot(temp, temp);
}

DirtyChunkComparator::DirtyChunkComparator(const Frustum& cameraFrustum) : CameraFrustum(cameraFrustum)
{
}

const Block* Chunk::GetBlockAt(const int x, const int y, const int z) const
{
    return BlockTypeList::GetBlockTypeData(GetBlockTypeAt(x, y, z));
}

EBlockType Chunk::GetBlockTypeAt(const int x, const int y, const int z) const
{
    return Blocks[static_cast<size_t>(EngineDefaults::GetChunkLocalIndex(x, y, z))];
}

bool Chunk::IsCoordsInsideChunk(const int x, const int y, const int z) const
{
    return x >= ChunkPosition.GetX() && y >= ChunkPosition.GetY() && z >= ChunkPosition.GetZ() && x < ChunkPosition.GetX() + CHUNK_WIDTH && y < ChunkPosition.GetY() + CHUNK_HEIGHT && z < ChunkPosition.GetZ() + CHUNK_DEPTH;
}

void Chunk::ResetDraw()
{
    Tessellation.Reset();
}

void Chunk::Draw()
{
    Tessellation.Draw();
    if (Tessellation.HasInitialized())
    {
        Tessellation.FreeMemory();
    }
}

void Chunk::GenerateTessellationData()
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
                if (const EBlockType type = Blocks[static_cast<size_t>(EngineDefaults::GetChunkLocalIndex(x, y, z))]; type != EBlockType::Air)
                {
                    DrawBlock(type, x, y, z);
                }
            }
        }
    }
}

void Chunk::SetBlockTypeAt(const int x, const int y, const int z, const EBlockType block)
{
    const auto index = static_cast<size_t>(EngineDefaults::GetChunkLocalIndex(x, y, z));
    const EBlockType check = Blocks[index];
    Blocks[index] = block;
    if (check != block)
    {
        GetWorld()->AddChunkAsDirty(this);
    }
}

bool Chunk::operator<(const Chunk& other) const
{
    if (other.TimeGotDirty - TimeGotDirty > 2.0)
    {
        return true;
    }
    return GetDistanceFromPlayer() < other.GetDistanceFromPlayer();
}

bool Chunk::ChunkInFrustum(const Frustum& frustum) const
{
    return frustum.CubeInFrustum(static_cast<float>(ChunkPosition.GetX() * CHUNK_WIDTH),
                                 static_cast<float>(ChunkPosition.GetY() * CHUNK_HEIGHT),
                                 static_cast<float>(ChunkPosition.GetZ() * CHUNK_DEPTH),
                                 static_cast<float>(ChunkPosition.GetX() * CHUNK_WIDTH + CHUNK_WIDTH),
                                 static_cast<float>(ChunkPosition.GetY() * CHUNK_HEIGHT + CHUNK_HEIGHT),
                                 static_cast<float>(ChunkPosition.GetZ() * CHUNK_DEPTH + CHUNK_DEPTH));
}

void Chunk::GotDirty()
{
    TimeGotDirty = glfwGetTime();
}

CustomFileManager& operator<<(CustomFileManager& fileManager, const Chunk& chunk)
{
    fileManager << chunk.ChunkPosition;
    for (const auto& block : chunk.Blocks)
    {
        fileManager << block;
    }
    return fileManager;
}

CustomFileManager& operator>>(CustomFileManager& fileManager, Chunk& chunk)
{
    for (auto& block : chunk.Blocks)
    {
        fileManager >> block;
    }
    return fileManager;
}
