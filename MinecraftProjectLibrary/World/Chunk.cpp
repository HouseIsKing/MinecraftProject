#include "Chunk.h"
#include "World.h"
#include "Blocks/BlockTypeList.h"


Chunk::Chunk(const int x, const int y, const int z) : Chunk(ChunkCoords(x, y, z))
{
}

Chunk::Chunk(const ChunkCoords& coords) : State(coords)
{
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
    return GetBlockTypeAt(EngineDefaults::GetChunkLocalIndex(x, y, z));
}

EBlockType Chunk::GetBlockTypeAt(const uint16_t index) const
{
    return State.GetState().Blocks[index];
}

const ChunkState& Chunk::GetState() const
{
    return State.GetState();
}

void Chunk::SetBlockTypeAt(const int x, const int y, const int z, const EBlockType block)
{
    SetBlockTypeAt(EngineDefaults::GetChunkLocalIndex(x, y, z), block);
}

void Chunk::SetBlockTypeAt(const uint16_t index, const EBlockType block)
{
    if (State.GetState().Blocks[index] != block)
    {
        State.SetBlockAt(index, block);
    }
}

void Chunk::RevertChunkChanges(const std::vector<uint8_t>& changes, size_t& pos)
{
    const auto& changeCount = EngineDefaults::ReadDataFromVector<uint16_t>(changes, pos);
    for (size_t i = 0; i < changeCount; i++)
    {
        const auto& index = EngineDefaults::ReadDataFromVector<uint16_t>(changes, pos);
        State.SetBlockAt(index, EngineDefaults::ReadDataFromVector<EBlockType>(changes, pos));
        pos += sizeof(EBlockType);
    }
}

void Chunk::AttachChunkChanges(std::vector<uint8_t>& changes) const
{
    State.WriteChangesToVector(changes);
}

void Chunk::ClearAllChanges()
{
    State.ClearAllChanges();
}

void Chunk::AttachChunkData(std::vector<uint8_t>& data) const
{
    EngineDefaults::EmplaceReplaceDataInVector(data, &State.GetState());
}
