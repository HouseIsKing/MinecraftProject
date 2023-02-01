#include "Chunk.h"

#include "World.h"

World* Chunk::TheWorld = nullptr;

void Chunk::SetWorld(::World* newWorld)
{
    TheWorld = newWorld;
}

const ChunkState& Chunk::GetChunkState() const
{
    return State;
}

Chunk::Chunk(const int x, const int y, const int z) : Tessellation(&ChunkTransform), State(ChunkCoords(x, y, z), {})
{
    TheWorld->AddChunkAsDirty(this);
}

Chunk::Chunk(const ChunkState& state) : Tessellation(&ChunkTransform), State(state)
{
    TheWorld->AddChunkAsDirty(this);
}

void Chunk::DrawBlock(const EBlockType blockType, const int x, const int y, const int z)
{
    const Block* block = BlockTypeList::GetBlockTypeData(blockType);
    const int finalX = x + State.ChunkPosition.X * EngineDefaults::CHUNK_WIDTH;
    const int finalY = y + State.ChunkPosition.Y * EngineDefaults::CHUNK_HEIGHT;
    const int finalZ = z + State.ChunkPosition.Z * EngineDefaults::CHUNK_DEPTH;
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
        const int brightness = TheWorld->GetBrightnessAt(x, y + 1, z);
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
                return !BlockTypeList::GetBlockTypeData(State.Blocks[static_cast<size_t>(EngineDefaults::GetChunkLocalIndex(x, y + 1, z))])->IsSolidBlock();
            }
            return !TheWorld->IsBlockSolid(x, y + 1, z);
        }
    case BlockFaces::Bottom:
        if (IsCoordsInsideChunk(x, y - 1, z))
        {
            return !BlockTypeList::GetBlockTypeData(State.Blocks[static_cast<size_t>(EngineDefaults::GetChunkLocalIndex(x, y - 1, z))])->IsSolidBlock();
        }
        return !TheWorld->IsBlockSolid(x, y - 1, z);
    case BlockFaces::North:
        if (IsCoordsInsideChunk(x, y, z + 1))
        {
            return !BlockTypeList::GetBlockTypeData(State.Blocks[static_cast<size_t>(EngineDefaults::GetChunkLocalIndex(x, y, z + 1))])->IsSolidBlock();
        }
        return !TheWorld->IsBlockSolid(x, y, z + 1);
    case BlockFaces::South:
        if (IsCoordsInsideChunk(x, y, z - 1))
        {
            return !BlockTypeList::GetBlockTypeData(State.Blocks[static_cast<size_t>(EngineDefaults::GetChunkLocalIndex(x, y, z - 1))])->IsSolidBlock();
        }
        return !TheWorld->IsBlockSolid(x, y, z - 1);
    case BlockFaces::East:
        if (IsCoordsInsideChunk(x + 1, y, z))
        {
            return !BlockTypeList::GetBlockTypeData(State.Blocks[static_cast<size_t>(EngineDefaults::GetChunkLocalIndex(x + 1, y, z))])->IsSolidBlock();
        }
        return !TheWorld->IsBlockSolid(x + 1, y, z);
    case BlockFaces::West:
        if (IsCoordsInsideChunk(x - 1, y, z))
        {
            return !BlockTypeList::GetBlockTypeData(State.Blocks[static_cast<size_t>(EngineDefaults::GetChunkLocalIndex(x - 1, y, z))])->IsSolidBlock();
        }
        return !TheWorld->IsBlockSolid(x - 1, y, z);
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
            brightness = TheWorld->GetBrightnessAt(x, y - 1, z);
            break;
        case BlockFaces::North:
            brightness = TheWorld->GetBrightnessAt(x, y, z + 1);
            break;
        case BlockFaces::South:
            brightness = TheWorld->GetBrightnessAt(x, y, z - 1);
            break;
        case BlockFaces::East:
            brightness = TheWorld->GetBrightnessAt(x + 1, y, z);
            break;
        case BlockFaces::West:
            brightness = TheWorld->GetBrightnessAt(x - 1, y, z);
            break;
        case BlockFaces::Top:
            brightness = TheWorld->GetBrightnessAt(x, y + 1, z);
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
    const auto* player = TheWorld->GetPlayer();
    if (player == nullptr)
    {
        return 0.0F;
    }
    const glm::vec3 temp = player->GetTransform().Position - glm::vec3(static_cast<float>(State.ChunkPosition.X * EngineDefaults::CHUNK_WIDTH) + EngineDefaults::CHUNK_WIDTH / 2.0F, static_cast<float>(State.ChunkPosition.Y * EngineDefaults::CHUNK_HEIGHT) + EngineDefaults::CHUNK_HEIGHT / 2.0F, static_cast<float>(State.ChunkPosition.Z * EngineDefaults::CHUNK_DEPTH) + EngineDefaults::CHUNK_DEPTH / 2.0F);
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
    return State.Blocks[static_cast<size_t>(EngineDefaults::GetChunkLocalIndex(x, y, z))];
}

EBlockType Chunk::GetBlockTypeAt(const uint16_t index) const
{
    return State.Blocks[index];
}

bool Chunk::IsCoordsInsideChunk(const int x, const int y, const int z) const
{
    return x >= State.ChunkPosition.X * EngineDefaults::CHUNK_WIDTH && y >= State.ChunkPosition.Y * EngineDefaults::CHUNK_HEIGHT && z >= State.ChunkPosition.Z * EngineDefaults::CHUNK_DEPTH && x < (State.ChunkPosition.X + 1) * EngineDefaults::CHUNK_WIDTH && y < (State.ChunkPosition.Y + 1) * EngineDefaults::CHUNK_HEIGHT && z < (State.ChunkPosition.Z + 1) * EngineDefaults::CHUNK_DEPTH;
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

void Chunk::SetBlockTypeAt(const int x, const int y, const int z, const EBlockType block)
{
    SetBlockTypeAt(EngineDefaults::GetChunkLocalIndex(x, y, z), block);
}

void Chunk::SetBlockTypeAt(const uint16_t index, const EBlockType block)
{
    const EBlockType check = State.Blocks[index];
    State.Blocks[index] = block;
    if (check != block)
    {
        TheWorld->AddChunkAsDirty(this);
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
    return frustum.CubeInFrustum(static_cast<float>(State.ChunkPosition.X * EngineDefaults::CHUNK_WIDTH),
                                 static_cast<float>(State.ChunkPosition.Y * EngineDefaults::CHUNK_HEIGHT),
                                 static_cast<float>(State.ChunkPosition.Z * EngineDefaults::CHUNK_DEPTH),
                                 static_cast<float>(State.ChunkPosition.X * EngineDefaults::CHUNK_WIDTH + EngineDefaults::CHUNK_WIDTH),
                                 static_cast<float>(State.ChunkPosition.Y * EngineDefaults::CHUNK_HEIGHT + EngineDefaults::CHUNK_HEIGHT),
                                 static_cast<float>(State.ChunkPosition.Z * EngineDefaults::CHUNK_DEPTH + EngineDefaults::CHUNK_DEPTH));
}

void Chunk::GotDirty()
{
    TimeGotDirty = glfwGetTime();
}

ChunkCoords Chunk::GetChunkPos() const
{
    return State.ChunkPosition;
}

void Chunk::ApplyChunkChanges(const std::vector<uint8_t>& changes, size_t& pos)
{
    const uint16_t changesCount = *reinterpret_cast<const uint16_t*>(&changes[pos]);
    pos += sizeof(uint16_t);
    for (int i = 0; i < changesCount; i++)
    {
        State.Blocks[*reinterpret_cast<const uint16_t*>(&changes[pos])] = *reinterpret_cast<const EBlockType*>(&changes[pos + sizeof(uint16_t)]);
        pos += sizeof(uint16_t) + sizeof(EBlockType);
    }
    TheWorld->AddChunkAsDirty(this);
}

void Chunk::RevertChunkState(const ChunkState& previousChunkState)
{
    State = previousChunkState;
}
