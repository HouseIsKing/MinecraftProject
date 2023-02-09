#pragma once
#include "BlockRenderer.h"

class PlanksBlockRenderer final : public BlockRenderer
{
public:
    PlanksBlockRenderer();
    [[nodiscard]] size_t GetIndexTextureSide(BlockFaces face) const override;
};
