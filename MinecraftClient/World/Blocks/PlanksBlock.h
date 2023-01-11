#pragma once
#include "Block.h"

class PlanksBlock final : public Block
{
public:
    PlanksBlock();
    [[nodiscard]] size_t GetIndexTextureSide(BlockFaces face) const override;
};
