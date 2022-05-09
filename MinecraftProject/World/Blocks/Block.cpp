//
// Created by amit on 4/22/2022.
//

#include "Block.h"

Block::Block() : Block(0, 0, 0, 1, 1, 1)
{
}

Block::Block(const float minX, const float minY, const float minZ, const float maxX, const float maxY, const float maxZ) : BlockBounds(minX, minY, minZ, maxX, maxY, maxZ)
{
}

Block::~Block() = default;

BoundingBox Block::GetBoundingBox() const
{
    return BlockBounds;
}