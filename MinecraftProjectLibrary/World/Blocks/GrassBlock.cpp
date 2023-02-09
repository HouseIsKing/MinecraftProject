#include "GrassBlock.h"
#include "World/World.h"

void GrassBlock::Tick(World* world, const int x, const int y, const int z) const
{
    Block::Tick(world, x, y, z);
    if (world->GetBrightnessAt(x, y + 1, z) == 1)
    {
        for (int i = 0; i < 4; i++)
        {
            const int targetX = x + world->TickRandomEngine.GetNext<int>(-1, 2);
            const int targetY = y + world->TickRandomEngine.GetNext<int>(-3, 2);
            if (const int targetZ = z + world->TickRandomEngine.GetNext<int>(-1, 2); world->GetBlockTypeAt(targetX, targetY, targetZ) == EBlockType::Dirt && world->GetBrightnessAt(targetX, targetY, targetZ) == 1)
            {
                world->PlaceBlockAt(targetX, targetY, targetZ, EBlockType::Grass);
            }
        }
    }
    else
    {
        world->PlaceBlockAt(x, y, z, EBlockType::Dirt);
    }
}
