#pragma once
#include "Block.h"

class GrassBlock final : public Block
{
public:
    GrassBlock() = default;
    void Tick(World* world, int x, int y, int z) const override;
};
