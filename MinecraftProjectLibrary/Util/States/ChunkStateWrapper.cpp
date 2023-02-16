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

void ChunkStateWrapper::WriteChangesToVector(std::vector<uint8_t>& changes, const uint16_t& changeType) const
{
    EngineDefaults::EmplaceReplaceDataInVector(changes, &OldState.Blocks[changeType]);
    EngineDefaults::EmplaceReplaceDataInVector(changes, &State.Blocks[changeType]);
}

void ChunkStateWrapper::ClearAllChanges(const uint16_t& change)
{
    OldState.Blocks[change] = State.Blocks[change];
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
    StateWrapper::WriteChangesToVector(changes);
}

void ChunkStateWrapper::ClearAllChanges()
{
    StateWrapper::ClearAllChanges();
}
