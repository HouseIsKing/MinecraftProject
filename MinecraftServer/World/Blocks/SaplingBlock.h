#pragma once
#include "Block.h"

class SaplingBlock final : public Block
{
public:
    SaplingBlock();
    [[nodiscard]] bool IsSolidBlock() const override;
    [[nodiscard]] bool IsBlockingLight() const override;
    void Tick(MultiPlayerWorld* world, int x, int y, int z) const override;
};
