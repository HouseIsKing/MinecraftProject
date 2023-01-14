#include "Chunk.h"
#include "MultiPlayerWorld.h"
#include "Util/EngineDefaults.h"

MultiPlayerWorld* Chunk::World = nullptr;

void Chunk::SetWorld(MultiPlayerWorld* newWorld)
{
    World = newWorld;
}

MultiPlayerWorld* Chunk::GetWorld()
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

const Block* Chunk::GetBlockAt(const int x, const int y, const int z) const
{
    return BlockTypeList::GetBlockTypeData(GetBlockTypeAt(x, y, z));
}

EBlockType Chunk::GetBlockTypeAt(const int x, const int y, const int z) const
{
    return Blocks[static_cast<size_t>(EngineDefaults::GetChunkLocalIndex(x, y, z))];
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
