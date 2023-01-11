#pragma once
#include "Block.h"

class CobblestoneBlock final : public Block
{
public:
    [[nodiscard]] size_t GetIndexTextureSide(BlockFaces face) const override;
    CobblestoneBlock();
};
