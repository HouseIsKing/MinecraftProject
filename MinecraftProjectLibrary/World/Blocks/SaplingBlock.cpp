#include "SaplingBlock.h"
#include "World/World.h"

SaplingBlock::SaplingBlock() = default;

bool SaplingBlock::IsSolidBlock() const
{
    return false;
}

bool SaplingBlock::IsBlockingLight() const
{
    return false;
}

void SaplingBlock::Tick(World* world, const int x, const int y, const int z) const
{
    if (const EBlockType below = world->GetBlockTypeAt(x, y - 1, z); (below != EBlockType::Dirt && below != EBlockType::Grass) || world->GetBrightnessAt(x, y, z) == 0)
    {
        world->PlaceBlockAt(x, y, z, EBlockType::Air);
    }
}
