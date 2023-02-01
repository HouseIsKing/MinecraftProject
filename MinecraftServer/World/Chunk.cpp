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

Chunk::Chunk(const int x, const int y, const int z)
{
    State.ChunkPosition = ChunkCoords(x, y, z);
    for (auto& block : State.Blocks)
    {
        block = EBlockType::Air;
    }
}

Chunk::Chunk(const ChunkState& state) : State(state)
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

const ChunkState& Chunk::GetChunkState() const
{
    return State;
}

void Chunk::SetBlockTypeAt(const int x, const int y, const int z, const EBlockType block)
{
    const auto index = static_cast<size_t>(EngineDefaults::GetChunkLocalIndex(x, y, z));
    State.Blocks[index] = block;
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
}

void Chunk::RevertChunkState(const ChunkState& previousChunkState)
{
    State = previousChunkState;
}

void Chunk::SendChunkToClient(const std::shared_ptr<Packet>& packet) const
{
    *packet << State.ChunkPosition.GetX() << State.ChunkPosition.GetY() << State.ChunkPosition.GetZ();
    for (const auto& block : State.Blocks)
    {
        *packet << static_cast<uint8_t>(block);
    }
}

CustomFileManager& operator<<(CustomFileManager& fileManager, const Chunk& chunk)
{
    fileManager << chunk.State.ChunkPosition;
    for (const auto& block : chunk.State.Blocks)
    {
        fileManager << block;
    }
    return fileManager;
}

CustomFileManager& operator>>(CustomFileManager& fileManager, Chunk& chunk)
{
    for (auto& block : chunk.State.Blocks)
    {
        fileManager >> block;
    }
    return fileManager;
}
