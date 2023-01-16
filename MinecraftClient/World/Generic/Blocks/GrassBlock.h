#pragma once
#include "Block.h"
#include "BlockTypeList.h"
#include "Util/EngineDefaults.h"

class GrassBlock final : public Block
{
public:
    [[nodiscard]] size_t GetIndexTextureSide(BlockFaces face) const override;
    void GenerateTessellationData(TessellationHelper& tessellationHelper, const BlockFaces& face, const float& x, const float& y, const float& z, const int& brightness, float r, float g, float b, float a) const override;
    GrassBlock();
    template <typename T>
    void Tick(T* world, int x, int y, int z) const;
};

template <typename T>
void GrassBlock::Tick(T* world, int x, int y, int z) const
{
    Block::Tick<T>(world, x, y, z);
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
