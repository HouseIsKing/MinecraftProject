#pragma once
#include "Block.h"

class DirtBlock final : public Block
{
public:
    [[nodiscard]] size_t GetIndexTextureSide(BlockFaces face) const override;
    DirtBlock();
};
