#include "EntityMeshBlock.h"

EntityMeshBlock::EntityMeshBlock(const float anchorPosX, const float anchorPosY, const float anchorPosZ, const float minX, const float minY, const float minZ, const float maxX, const float maxY, const float maxZ) :
	EntityBlockBoundingBox(minX, minY, minZ, maxX, maxY, maxZ)
{
	EntityBlockTransform.Position = glm::vec3(anchorPosX, anchorPosY, anchorPosZ);
}

TransformStruct& EntityMeshBlock::GetTransform()
{
	return EntityBlockTransform;
}