#pragma once
#include "Util/BoundingBox.h"
#include "Util/EngineDefaults.h"

class EntityMeshBlock
{
	TransformStruct EntityBlockTransform;
	BoundingBox EntityBlockBoundingBox;

public:
	EntityMeshBlock(float anchorPosX, float anchorPosY, float anchorPosZ, float minX, float minY, float minZ, float maxX, float maxY, float maxZ);
	[[nodiscard]] TransformStruct& GetTransform();
};
