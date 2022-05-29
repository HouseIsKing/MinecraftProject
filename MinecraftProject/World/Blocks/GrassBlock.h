//
// Created by amit on 4/22/2022.
//

#pragma once
#include "Block.h"

class GrassBlock final : public Block
{
public:
    void GenerateTessellationData(TessellationHelper& tessellationHelper, const BlockFaces& face, const float& x, const float& y, const float& z, const int& brightness) const override;
    GrassBlock();
};
