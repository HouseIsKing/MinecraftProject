#include "Block.h"

Block::Block() : Block(0, 0, 0, 1, 1, 1)
{
}

Block::Block(const float minX, const float minY, const float minZ, const float maxX, const float maxY, const float maxZ) : BlockBounds(minX, minY, minZ, maxX, maxY, maxZ)
{
}

BoundingBox Block::GetBoundingBox() const
{
	return BlockBounds;
}

void Block::Tick(World* /*world*/, int /*x*/, int /*y*/, int /*z*/) const
{
}

void Block::OnBreak(World* /*world*/, const int /*x*/, const int /*y*/, const int /*z*/) const
{
}

bool Block::IsSolidBlock() const
{
	return true;
}

bool Block::IsBlockingLight() const
{
	return true;
}
