#pragma once
#include "Block.h"

class AirBlock final : public Block
{
public:
	AirBlock();
	void GenerateTessellationData(TessellationHelper& tessellationHelper, const BlockFaces& face, const float& x, const float& y, const float& z, const int& brightness) const override;
	[[nodiscard]] size_t GetIndexTextureSide(BlockFaces face) const override;
	void GenerateTessellationData(GuiTessellation& tessellationHelper, const BlockFaces& face, mat4x4 transformationMatrix) const override;
};

