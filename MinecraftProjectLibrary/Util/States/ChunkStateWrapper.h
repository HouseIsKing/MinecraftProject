#pragma once
#include "ChunkState.h"
#include "StateWrapper.h"

class ChunkStateWrapper final : public StateWrapper<ChunkState, uint16_t>
{
protected:
    void WriteChangesToVector(std::vector<uint8_t>& changes, const uint16_t& changeType) const override;
    void ClearAllChanges(const uint16_t& change) override;

public:
    explicit ChunkStateWrapper(ChunkCoords chunkPosition);
    explicit ChunkStateWrapper(const ChunkState& otherState);
    void SetBlockAt(uint16_t index, EBlockType type);
    void WriteChangesToVector(std::vector<uint8_t>& changes) const override;
    void ClearAllChanges() override;
};
