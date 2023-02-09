#pragma once
#include "BlockRenderer.h"

class GrassBlockRenderer final : public BlockRenderer
{
public:
    [[nodiscard]] size_t GetIndexTextureSide(BlockFaces face) const override;
    void GenerateTessellationData(const Block* block, TessellationHelper& tessellationHelper, const BlockFaces& face, const float& x, const float& y, const float& z, const int& brightness, float r, float g, float b, float a) const override;
    GrassBlockRenderer();
};
