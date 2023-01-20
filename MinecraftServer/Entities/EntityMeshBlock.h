#pragma once
#include <array>

#include "Util/BoundingBox.h"
#include "Util/Transform.h"

class EntityMeshBlock
{
	Transform EntityBlockTransform;
	BoundingBox EntityBlockBoundingBox;

public:
	EntityMeshBlock(float anchorPosX, float anchorPosY, float anchorPosZ, float minX, float minY, float minZ, float maxX, float maxY, float maxZ, Transform* parent);
	[[nodiscard]] Transform& GetTransform();
};
