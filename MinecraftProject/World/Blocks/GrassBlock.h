//
// Created by amit on 4/22/2022.
//

#pragma once
#include "Block.h"

using std::unique_ptr;

class GrassBlock final : public Block
{
public:
    void GenerateTessellationData(TessellationHelper& tessellationHelper, const BlockFaces& face, const float& x, const float& y, const float& z, const float& brightness) const override;
    GrassBlock();
};
