#pragma once
#include "Block.h"

class AirBlock final : public Block
{
public:
	AirBlock();
	void GenerateTessellationData(TessellationHelper& tessellationHelper, const BlockFaces& face, const float& x, const float& y, const float& z) const override;
};

