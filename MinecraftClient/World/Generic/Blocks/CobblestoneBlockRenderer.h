#pragma once
#include "BlockRenderer.h"

class CobblestoneBlockRenderer final : public BlockRenderer
{
public:
    [[nodiscard]] size_t GetIndexTextureSide(BlockFaces face) const override;
    CobblestoneBlockRenderer();
};
