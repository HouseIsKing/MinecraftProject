//
// Created by amit on 4/22/2022.
//

#pragma once
#include "Util/BoundingBox.h"
class MultiPlayerWorld;

enum class BlockFaces
{
    Top,
    Bottom,
    West,
    East,
    North,
    South
};

enum class DrawType
{
    Default,
    Sapling
};

class Block
{
protected:
    BoundingBox BlockBounds;
    Block();
    Block(float minX, float minY, float minZ, float maxX, float maxY, float maxZ);

public:
    virtual ~Block();
    Block(const Block&) = delete;
    Block& operator=(const Block&) = delete;
    Block(Block&&) = delete;
    Block& operator=(Block&&) = delete;
    [[nodiscard]] BoundingBox GetBoundingBox() const;
    virtual void Tick(MultiPlayerWorld* world, int x, int y, int z) const;
    virtual void OnBreak(MultiPlayerWorld* world, int x, int y, int z) const;
    [[nodiscard]] virtual bool IsSolidBlock() const;
    [[nodiscard]] virtual bool IsBlockingLight() const;
};
