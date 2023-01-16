#pragma once
#include "Block.h"
#include "BlockTypeList.h"

class SaplingBlock final : public Block
{
public:
    SaplingBlock();
    [[nodiscard]] size_t GetIndexTextureSide(BlockFaces face) const override;
    void GenerateTessellationDataForAllFaces(TessellationHelper& tessellationHelper, const float& x, const float& y, const float& z, const int& brightness, float r, float g, float b, float a) const override;
    void GenerateTessellationData(TessellationHelper& tessellationHelper, const BlockFaces& face, const float& x, const float& y, const float& z, const int& brightness, float r, float g, float b, float a) const override;
    void GenerateTessellationData(GuiTessellation& tessellationHelper, const BlockFaces& face, glm::mat4x4 transformationMatrix) const override;
    [[nodiscard]] DrawType GetDrawType() const override;
    [[nodiscard]] bool IsSolidBlock() const override;
    [[nodiscard]] bool IsBlockingLight() const override;
    template <typename T>
    void Tick(T* world, int x, int y, int z) const;
};

template <typename T>
void SaplingBlock::Tick(T* world, int x, int y, int z) const
{
    if (const EBlockType below = world->GetBlockTypeAt(x, y - 1, z); (below != EBlockType::Dirt && below != EBlockType::Grass) || world->GetBrightnessAt(x, y, z) == 0)
    {
        world->PlaceBlockAt(x, y, z, EBlockType::Air);
    }
}
