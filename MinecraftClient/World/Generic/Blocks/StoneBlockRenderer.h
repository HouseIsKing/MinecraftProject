#pragma once
#include "BlockRenderer.h"

class StoneBlockRenderer final : public BlockRenderer
{
public:
    StoneBlockRenderer();
    [[nodiscard]] size_t GetIndexTextureSide(BlockFaces face) const override;
};
