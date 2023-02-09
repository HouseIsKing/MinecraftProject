#pragma once
#include "BlockRenderer.h"

class DirtBlockRenderer final : public BlockRenderer
{
public:
    [[nodiscard]] size_t GetIndexTextureSide(BlockFaces face) const override;
    DirtBlockRenderer();
};
