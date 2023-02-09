#pragma once
#include "BlockRenderer.h"

class SaplingBlockRenderer final : public BlockRenderer
{
public:
    SaplingBlockRenderer();
    [[nodiscard]] size_t GetIndexTextureSide(BlockFaces face) const override;
    void GenerateTessellationDataForAllFaces(const Block* block, TessellationHelper& tessellationHelper, const float& x, const float& y, const float& z, const int& brightness, float r, float g, float b, float a) const override;
    void GenerateTessellationData(const Block* block, TessellationHelper& tessellationHelper, const BlockFaces& face, const float& x, const float& y, const float& z, const int& brightness, float r, float g, float b, float a) const override;
    void GenerateTessellationData(const Block* block, GuiTessellation& tessellationHelper, const BlockFaces& face, glm::mat4x4 transformationMatrix) const override;
    [[nodiscard]] DrawType GetDrawType() const override;
};
