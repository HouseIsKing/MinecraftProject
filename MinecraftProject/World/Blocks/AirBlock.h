#pragma once
#include "Block.h"

class AirBlock final : public Block
{
public:
	AirBlock();
	void GenerateTessellationData(TessellationHelper& tessellationHelper, const BlockFaces& face, const float& x, const float& y, const float& z, const int& brightness, float r, float g, float b, float a) const override;
	[[nodiscard]] size_t GetIndexTextureSide(BlockFaces face) const override;
	void GenerateTessellationData(GuiTessellation& tessellationHelper, const BlockFaces& face, mat4x4 transformationMatrix) const override;
	[[nodiscard]] bool IsSolidBlock() const override;
	[[nodiscard]] bool IsBlockingLight() const override;
};

