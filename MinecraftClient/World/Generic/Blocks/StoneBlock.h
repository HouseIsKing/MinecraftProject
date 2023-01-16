#pragma once
#include "Block.h"

class StoneBlock final : public Block
{
public:
    StoneBlock();
    [[nodiscard]] size_t GetIndexTextureSide(BlockFaces face) const override;
};
