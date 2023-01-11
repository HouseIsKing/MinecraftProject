#pragma once
#include "Block.h"

class SaplingBlock final : public Block
{
public:
    SaplingBlock();
    [[nodiscard]] size_t GetIndexTextureSide(BlockFaces face) const override;
    void GenerateTessellationDataForAllFaces(TessellationHelper& tessellationHelper, const float& x, const float& y, const float& z, const int& brightness, float r, float g, float b, float a) const override;
    void GenerateTessellationData(TessellationHelper& tessellationHelper, const BlockFaces& face, const float& x, const float& y, const float& z, const int& brightness, float r, float g, float b, float a) const override;
    void GenerateTessellationData(GuiTessellation& tessellationHelper, const BlockFaces& face, mat4x4 transformationMatrix) const override;
    [[nodiscard]] DrawType GetDrawType() const override;
    [[nodiscard]] bool IsSolidBlock() const override;
    [[nodiscard]] bool IsBlockingLight() const override;
    void Tick(SinglePlayerWorld* world, int x, int y, int z) const override;
};
