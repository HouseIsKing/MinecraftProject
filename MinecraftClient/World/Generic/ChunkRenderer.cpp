#include "ChunkRenderer.h"

#include "Blocks/BlockRendererList.h"
#include "ClientWorld.h"
#include "World/Blocks/BlockTypeList.h"

ChunkRenderer::ChunkRenderer(const ChunkState& state) : State(state), Tessellation(&ChunkTransform)
{
    const ChunkCoords& coords = state.ChunkPosition;
    ChunkTransform.Position = glm::vec3(static_cast<float>(coords.GetX() * EngineDefaults::CHUNK_WIDTH), static_cast<float>(coords.GetY() * EngineDefaults::CHUNK_HEIGHT), static_cast<float>(coords.GetZ() * EngineDefaults::CHUNK_DEPTH));
}

void ChunkRenderer::DrawBlock(const EBlockType blockType, const int x, const int y, const int z)
{
    switch (const BlockRenderer* block = BlockRendererList::GetBlockRenderer(blockType); block->GetDrawType())
    {
    case DrawType::Default:
        DrawDefaultBlock(block, x, y, z, blockType);
        break;
    case DrawType::Sapling:
        DrawSaplingBlock(block, x, y, z, blockType);
        break;
    }
}

void ChunkRenderer::DrawSaplingBlock(const BlockRenderer* block, const int x, const int y, const int z, const EBlockType blockType)
{
    const int globalX = x + static_cast<int>(std::round(ChunkTransform.Position.x));
    const int globalY = y + static_cast<int>(std::round(ChunkTransform.Position.y));
    const int globalZ = z + static_cast<int>(std::round(ChunkTransform.Position.z));
    if (IsSideNeedToBeDrawn(BlockFaces::Top, x, y, z) || IsSideNeedToBeDrawn(BlockFaces::Bottom, x, y, z) || IsSideNeedToBeDrawn(BlockFaces::North, x, y, z) || IsSideNeedToBeDrawn(BlockFaces::South, x, y, z) || IsSideNeedToBeDrawn(BlockFaces::East, x, y, z) || IsSideNeedToBeDrawn(BlockFaces::West, x, y, z))
    {
        const int brightness = World::GetWorld()->GetBrightnessAt(globalX, globalY + 1, globalZ);
        block->GenerateTessellationData(BlockTypeList::GetBlockTypeData(blockType), Tessellation, BlockFaces::Top, static_cast<float>(x), static_cast<float>(y), static_cast<float>(z), brightness);
    }
}

bool ChunkRenderer::IsSideNeedToBeDrawn(const BlockFaces& face, const int x, const int y, const int z) const
{
    const int globalX = x + static_cast<int>(std::round(ChunkTransform.Position.x));
    const int globalY = y + static_cast<int>(std::round(ChunkTransform.Position.y));
    const int globalZ = z + static_cast<int>(std::round(ChunkTransform.Position.z));
    switch (face)
    {
    case BlockFaces::Top:
        {
            if (IsCoordsInsideChunk(x, y + 1, z))
            {
                return !BlockTypeList::GetBlockTypeData(State.Blocks[static_cast<size_t>(EngineDefaults::GetChunkLocalIndex(x, y + 1, z))])->IsSolidBlock();
            }
            return !World::GetWorld()->IsBlockSolid(globalX, globalY + 1, globalZ);
        }
    case BlockFaces::Bottom:
        if (IsCoordsInsideChunk(x, y - 1, z))
        {
            return !BlockTypeList::GetBlockTypeData(State.Blocks[static_cast<size_t>(EngineDefaults::GetChunkLocalIndex(x, y - 1, z))])->IsSolidBlock();
        }
        return !World::GetWorld()->IsBlockSolid(globalX, globalY - 1, globalZ);
    case BlockFaces::North:
        if (IsCoordsInsideChunk(x, y, z + 1))
        {
            return !BlockTypeList::GetBlockTypeData(State.Blocks[static_cast<size_t>(EngineDefaults::GetChunkLocalIndex(x, y, z + 1))])->IsSolidBlock();
        }
        return !World::GetWorld()->IsBlockSolid(globalX, globalY, globalZ + 1);
    case BlockFaces::South:
        if (IsCoordsInsideChunk(x, y, z - 1))
        {
            return !BlockTypeList::GetBlockTypeData(State.Blocks[static_cast<size_t>(EngineDefaults::GetChunkLocalIndex(x, y, z - 1))])->IsSolidBlock();
        }
        return !World::GetWorld()->IsBlockSolid(globalX, globalY, globalZ - 1);
    case BlockFaces::East:
        if (IsCoordsInsideChunk(x + 1, y, z))
        {
            return !BlockTypeList::GetBlockTypeData(State.Blocks[static_cast<size_t>(EngineDefaults::GetChunkLocalIndex(x + 1, y, z))])->IsSolidBlock();
        }
        return !World::GetWorld()->IsBlockSolid(globalX + 1, globalY, globalZ);
    case BlockFaces::West:
        if (IsCoordsInsideChunk(x - 1, y, z))
        {
            return !BlockTypeList::GetBlockTypeData(State.Blocks[static_cast<size_t>(EngineDefaults::GetChunkLocalIndex(x - 1, y, z))])->IsSolidBlock();
        }
        return !World::GetWorld()->IsBlockSolid(globalX - 1, globalY, globalZ);
    }
    return false;
}

void ChunkRenderer::DrawDefaultBlock(const BlockRenderer* block, const int x, const int y, const int z, const EBlockType blockType)
{
    const int globalX = x + static_cast<int>(std::round(ChunkTransform.Position.x));
    const int globalY = y + static_cast<int>(std::round(ChunkTransform.Position.y));
    const int globalZ = z + static_cast<int>(std::round(ChunkTransform.Position.z));
    for (int face = static_cast<int>(BlockFaces::Top); face <= static_cast<int>(BlockFaces::South); face++)
    {
        int brightness;
        switch (static_cast<BlockFaces>(face))
        {
        case BlockFaces::Bottom:
            brightness = World::GetWorld()->GetBrightnessAt(globalX, globalY - 1, globalZ);
            break;
        case BlockFaces::North:
            brightness = World::GetWorld()->GetBrightnessAt(globalX, globalY, globalZ + 1);
            break;
        case BlockFaces::South:
            brightness = World::GetWorld()->GetBrightnessAt(globalX, globalY, globalZ - 1);
            break;
        case BlockFaces::East:
            brightness = World::GetWorld()->GetBrightnessAt(globalX + 1, globalY, globalZ);
            break;
        case BlockFaces::West:
            brightness = World::GetWorld()->GetBrightnessAt(globalX - 1, globalY, globalZ);
            break;
        case BlockFaces::Top:
            brightness = World::GetWorld()->GetBrightnessAt(globalX, globalY + 1, globalZ);
            break;
        }
        if (IsSideNeedToBeDrawn(static_cast<BlockFaces>(face), x, y, z))
        {
            block->GenerateTessellationData(BlockTypeList::GetBlockTypeData(blockType), Tessellation, static_cast<BlockFaces>(face), static_cast<float>(x), static_cast<float>(y), static_cast<float>(z), brightness);
        }
    }
}

float ChunkRenderer::GetDistanceFromPlayer() const
{
    const auto& player = ClientWorld::GetWorld()->GetPlayer();
    const glm::vec3 temp = player->GetState().EntityTransform.Position - ChunkTransform.Position - glm::vec3(EngineDefaults::CHUNK_WIDTH / 2.0F, EngineDefaults::CHUNK_HEIGHT / 2.0F, EngineDefaults::CHUNK_DEPTH / 2.0F);
    return dot(temp, temp);
}

DirtyChunkComparator::DirtyChunkComparator(const Frustum& cameraFrustum) : CameraFrustum(cameraFrustum)
{
}

bool ChunkRenderer::IsCoordsInsideChunk(const int x, const int y, const int z)
{
    return x >= 0 && y >= 0 && z >= 0 && x < EngineDefaults::CHUNK_WIDTH && y < EngineDefaults::CHUNK_HEIGHT && z < EngineDefaults::CHUNK_DEPTH;
}

void ChunkRenderer::ResetDraw()
{
    Tessellation.Reset();
}

void ChunkRenderer::Draw()
{
    Tessellation.Draw();
    if (Tessellation.HasInitialized())
    {
        Tessellation.FreeMemory();
    }
}

void ChunkRenderer::GenerateTessellationData()
{
    if (Tessellation.HasInitialized())
    {
        ResetDraw();
    }
    for (int x = 0; x < EngineDefaults::CHUNK_WIDTH; x++)
    {
        for (int y = 0; y < EngineDefaults::CHUNK_HEIGHT; y++)
        {
            for (int z = 0; z < EngineDefaults::CHUNK_DEPTH; z++)
            {
                if (const EBlockType type = State.Blocks[static_cast<size_t>(EngineDefaults::GetChunkLocalIndex(x, y, z))]; type != EBlockType::Air)
                {
                    DrawBlock(type, x, y, z);
                }
            }
        }
    }
}

bool ChunkRenderer::operator<(const ChunkRenderer& other) const
{
    if (other.TimeGotDirty - TimeGotDirty > 2.0)
    {
        return true;
    }
    return GetDistanceFromPlayer() < other.GetDistanceFromPlayer();
}

bool ChunkRenderer::ChunkInFrustum(const Frustum& frustum) const
{
    return frustum.CubeInFrustum(ChunkTransform.Position.x,
                                 ChunkTransform.Position.y,
                                 ChunkTransform.Position.z,
                                 ChunkTransform.Position.x + EngineDefaults::CHUNK_WIDTH,
                                 ChunkTransform.Position.y + EngineDefaults::CHUNK_HEIGHT,
                                 ChunkTransform.Position.z + EngineDefaults::CHUNK_DEPTH);
}

void ChunkRenderer::GotDirty()
{
    TimeGotDirty = glfwGetTime();
}
