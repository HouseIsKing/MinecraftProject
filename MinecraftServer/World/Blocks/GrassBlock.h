#pragma once
#include "Block.h"

class GrassBlock final : public Block
{
public:
    GrassBlock();
    void Tick(MultiPlayerWorld* world, int x, int y, int z) const override;
};
