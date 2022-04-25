#pragma once
#include "Block.h"
class AirBlock : public Block
{
public:
	AirBlock();
	void generateTessellationData(TessellationHelper& tessellationHelper, const BlockFaces& face, const float& x, const float& y, const float& z) const override;
};

