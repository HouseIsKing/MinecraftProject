#include "ChunkStateWrapper.h"


ChunkStateWrapper::ChunkStateWrapper(const ChunkCoords chunkPosition)
{
    State.ChunkPosition = chunkPosition;
    OldState.ChunkPosition = chunkPosition;
}

ChunkStateWrapper::ChunkStateWrapper(const ChunkState& otherState) : StateWrapper(otherState)
{
}

void ChunkStateWrapper::SetBlockAt(uint16_t index, const EBlockType type)
{
    Changes.emplace(index);
    State.Blocks[index] = type;
}

void ChunkStateWrapper::WriteChangesToVector(std::vector<uint8_t>& changes) const
{
    if (Changes.empty())
    {
        return;
    }
    constexpr auto changeType = EChangeType::ChunkState;
    EngineDefaults::EmplaceReplaceDataInVector(changes, &changeType);
    EngineDefaults::EmplaceReplaceDataInVector(changes, &State.ChunkPosition);
    const auto changeCount = static_cast<uint16_t>(Changes.size());
    EngineDefaults::EmplaceReplaceDataInVector(changes, &changeCount);
    for (const uint16_t& index : Changes)
    {
        EngineDefaults::EmplaceReplaceDataInVector(changes, &index);
        EngineDefaults::EmplaceReplaceDataInVector(changes, &OldState.Blocks[index]);
        EngineDefaults::EmplaceReplaceDataInVector(changes, &State.Blocks[index]);
    }
}

void ChunkStateWrapper::ClearAllChanges()
{
    for (const uint16_t& index : Changes)
    {
        OldState.Blocks[index] = State.Blocks[index];
    }
    Changes.clear();
}
