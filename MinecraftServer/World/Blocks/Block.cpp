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

void Block::Tick(MultiPlayerWorld* /*world*/, int /*x*/, int /*y*/, int /*z*/) const
{
}

void Block::OnBreak(MultiPlayerWorld* /*world*/, const int /*x*/, const int /*y*/, const int /*z*/) const
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
