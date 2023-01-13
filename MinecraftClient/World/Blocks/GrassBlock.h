#pragma once
#include "Block.h"

class GrassBlock final : public Block
{
public:
    [[nodiscard]] size_t GetIndexTextureSide(BlockFaces face) const override;
    void GenerateTessellationData(TessellationHelper& tessellationHelper, const BlockFaces& face, const float& x, const float& y, const float& z, const int& brightness, float r, float g, float b, float a) const override;
    GrassBlock();
    void Tick(SinglePlayerWorld* world, int x, int y, int z) const override;
};
