#include "Chunk.h"
#include "MultiPlayerWorld.h"
#include "Network/Packets/Packet.h"
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
    Blocks[index] = block;
}

void Chunk::SendChunkToClient(const std::shared_ptr<Packet>& packet) const
{
    *packet << ChunkPosition.GetX() << ChunkPosition.GetY() << ChunkPosition.GetZ();
    for (const auto& block : Blocks)
    {
        *packet << static_cast<uint8_t>(block);
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
