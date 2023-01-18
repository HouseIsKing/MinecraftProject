#include "GrassBlock.h"
#include "Util/EngineDefaults.h"
#include "World/MultiPlayerWorld.h"


GrassBlock::GrassBlock() = default;

void GrassBlock::Tick(MultiPlayerWorld* world, const int x, const int y, const int z) const
{
    Block::Tick(world, x, y, z);
    if (world->GetBrightnessAt(x, y + 1, z) == 1)
    {
        for (int i = 0; i < 4; i++)
        {
            const int targetX = x + EngineDefaults::GetNext<int>(-1, 2);
            const int targetY = y + EngineDefaults::GetNext<int>(-3, 2);
            if (const int targetZ = z + EngineDefaults::GetNext<int>(-1, 2); world->GetBlockTypeAt(targetX, targetY, targetZ) == EBlockType::Dirt && world->GetBrightnessAt(targetX, targetY, targetZ) == 1)
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