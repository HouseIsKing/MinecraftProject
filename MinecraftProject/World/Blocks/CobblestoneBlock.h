#pragma once
#include "Block.h"

class CobblestoneBlock final : public Block
{
public:
    void GenerateTessellationData(TessellationHelper& tessellationHelper, const BlockFaces& face, const float& x, const float& y, const float& z, const float& brightness) const override;
    CobblestoneBlock();
};
