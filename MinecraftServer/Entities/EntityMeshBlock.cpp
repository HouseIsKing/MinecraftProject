#include "EntityMeshBlock.h"
#include "Util/EngineDefaults.h"

EntityMeshBlock::EntityMeshBlock(const float anchorPosX, const float anchorPosY, const float anchorPosZ, const float minX, const float minY, const float minZ, const float maxX, const float maxY, const float maxZ, Transform* parent) :
	EntityBlockTransform(parent), EntityBlockBoundingBox(minX, minY, minZ, maxX, maxY, maxZ)
{
	EntityBlockTransform.SetPosition(anchorPosX, anchorPosY, anchorPosZ);
}

Transform& EntityMeshBlock::GetTransform()
{
	return EntityBlockTransform;
}