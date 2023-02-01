#include "EntityMeshBlock.h"

EntityMeshBlock::EntityMeshBlock(const std::string& meshTexture, const float anchorPosX, const float anchorPosY, const float anchorPosZ, const float minX, const float minY, const float minZ, const float maxX, const float maxY, const float maxZ, const std::array<float, 24>& uvs, TessellationHelper& tessellationHelper) : Block(minX, minY, minZ, maxX, maxY, maxZ),
	TrianglesStartPos(0), BlockUVs(uvs), BlockVerticesPositions(), BlockTransformId(tessellationHelper.AddTransform(new Transform(&CurrentTransform, tessellationHelper.GetTransform(0)))),
	CurrentTransform(glm::vec3(anchorPosX, anchorPosY, anchorPosZ)), EntityBlockTransform(*tessellationHelper.GetTransform(BlockTransformId)), PrevTransform(CurrentTransform)
{
	IndexTextures.push_back(EngineDefaults::RegisterTexture(Texture::LoadTexture(meshTexture)));
}

void EntityMeshBlock::GenerateTessellationData(TessellationHelper& tessellationHelper, const int brightness) const
{
	GenerateTessellationData(tessellationHelper, BlockFaces::Top, 0, 0, 0, brightness, 1.0F, 1.0F, 1.0F, 1.0F);
	GenerateTessellationData(tessellationHelper, BlockFaces::Bottom, 0, 0, 0, brightness, 1.0F, 1.0F, 1.0F, 1.0F);
	GenerateTessellationData(tessellationHelper, BlockFaces::South, 0, 0, 0, brightness, 1.0F, 1.0F, 1.0F, 1.0F);
	GenerateTessellationData(tessellationHelper, BlockFaces::North, 0, 0, 0, brightness, 1.0F, 1.0F, 1.0F, 1.0F);
	GenerateTessellationData(tessellationHelper, BlockFaces::East, 0, 0, 0, brightness, 1.0F, 1.0F, 1.0F, 1.0F);
	GenerateTessellationData(tessellationHelper, BlockFaces::West, 0, 0, 0, brightness, 1.0F, 1.0F, 1.0F, 1.0F);
}

void EntityMeshBlock::GenerateTessellationData(GuiTessellation& /*tessellationHelper*/, const BlockFaces& /*face*/, glm::mat4x4 /*transformationMatrix*/) const
{
}

void EntityMeshBlock::GenerateTessellationData(TessellationHelper& tessellationHelper, const BlockFaces& face,
                                               const float& /*x*/, const float& /*y*/, const float& /*z*/, const int& brightness, const float r, const float g, const float b, const float a) const
{
	uint16_t triangleIndex1 = 0;
	uint16_t triangleIndex2 = 0;
	uint16_t triangleIndex3 = 0;
	uint16_t triangleIndex4 = 0;
	const float ambientLightFactor = EngineDefaults::ConvertLightLevelToAmbient(brightness);
	switch (face)
	{
	case BlockFaces::Top:
		triangleIndex1 = tessellationHelper.AddVertex(Vertex(BlockVerticesPositions[2], BlockUVs[0], BlockUVs[3], r * ambientLightFactor, g * ambientLightFactor, b * ambientLightFactor, a, IndexTextures[0], 0.0F, 1.0F, 0.0F, brightness));
		triangleIndex2 = tessellationHelper.AddVertex(Vertex(BlockVerticesPositions[3], BlockUVs[2], BlockUVs[3], r * ambientLightFactor, g * ambientLightFactor, b * ambientLightFactor, a, IndexTextures[0], 0.0F, 1.0F, 0.0F, brightness));
		triangleIndex3 = tessellationHelper.AddVertex(Vertex(BlockVerticesPositions[6], BlockUVs[0], BlockUVs[1], r * ambientLightFactor, g * ambientLightFactor, b * ambientLightFactor, a, IndexTextures[0], 0.0F, 1.0F, 0.0F, brightness));
		triangleIndex4 = tessellationHelper.AddVertex(Vertex(BlockVerticesPositions[7], BlockUVs[2], BlockUVs[1], r * ambientLightFactor, g * ambientLightFactor, b * ambientLightFactor, a, IndexTextures[0], 0.0F, 1.0F, 0.0F, brightness));
		break;
	case BlockFaces::Bottom:
		triangleIndex1 = tessellationHelper.AddVertex(Vertex(BlockVerticesPositions[0], BlockUVs[4], BlockUVs[5], r * ambientLightFactor, g * ambientLightFactor, b * ambientLightFactor, a, IndexTextures[0], 0.0F, -1.0F, 0.0F, brightness));
		triangleIndex2 = tessellationHelper.AddVertex(Vertex(BlockVerticesPositions[4], BlockUVs[4], BlockUVs[7], r * ambientLightFactor, g * ambientLightFactor, b * ambientLightFactor, a, IndexTextures[0], 0.0F, -1.0F, 0.0F, brightness));
		triangleIndex3 = tessellationHelper.AddVertex(Vertex(BlockVerticesPositions[1], BlockUVs[6], BlockUVs[5], r * ambientLightFactor, g * ambientLightFactor, b * ambientLightFactor, a, IndexTextures[0], 0.0F, -1.0F, 0.0F, brightness));
		triangleIndex4 = tessellationHelper.AddVertex(Vertex(BlockVerticesPositions[5], BlockUVs[6], BlockUVs[7], r * ambientLightFactor, g * ambientLightFactor, b * ambientLightFactor, a, IndexTextures[0], 0.0F, -1.0F, 0.0F, brightness));
		break;
	case BlockFaces::South:
		triangleIndex1 = tessellationHelper.AddVertex(Vertex(BlockVerticesPositions[0], BlockUVs[20], BlockUVs[21], r * ambientLightFactor, g * ambientLightFactor, b * ambientLightFactor, a, IndexTextures[0], 0.0F, 0.0F, -1.0F, brightness));
		triangleIndex2 = tessellationHelper.AddVertex(Vertex(BlockVerticesPositions[2], BlockUVs[20], BlockUVs[23], r * ambientLightFactor, g * ambientLightFactor, b * ambientLightFactor, a, IndexTextures[0], 0.0F, 0.0F, -1.0F, brightness));
		triangleIndex3 = tessellationHelper.AddVertex(Vertex(BlockVerticesPositions[4], BlockUVs[22], BlockUVs[21], r * ambientLightFactor, g * ambientLightFactor, b * ambientLightFactor, a, IndexTextures[0], 0.0F, 0.0F, -1.0F, brightness));
		triangleIndex4 = tessellationHelper.AddVertex(Vertex(BlockVerticesPositions[6], BlockUVs[22], BlockUVs[23], r * ambientLightFactor, g * ambientLightFactor, b * ambientLightFactor, a, IndexTextures[0], 0.0F, 0.0F, -1.0F, brightness));
		break;
	case BlockFaces::North:
		triangleIndex1 = tessellationHelper.AddVertex(Vertex(BlockVerticesPositions[1], BlockUVs[18], BlockUVs[17], r * ambientLightFactor, g * ambientLightFactor, b * ambientLightFactor, a, IndexTextures[0], 0.0F, 0.0F, 1.0F, brightness));
		triangleIndex2 = tessellationHelper.AddVertex(Vertex(BlockVerticesPositions[5], BlockUVs[16], BlockUVs[17], r * ambientLightFactor, g * ambientLightFactor, b * ambientLightFactor, a, IndexTextures[0], 0.0F, 0.0F, 1.0F, brightness));
		triangleIndex3 = tessellationHelper.AddVertex(Vertex(BlockVerticesPositions[3], BlockUVs[18], BlockUVs[19], r * ambientLightFactor, g * ambientLightFactor, b * ambientLightFactor, a, IndexTextures[0], 0.0F, 0.0F, 1.0F, brightness));
		triangleIndex4 = tessellationHelper.AddVertex(Vertex(BlockVerticesPositions[7], BlockUVs[16], BlockUVs[19], r * ambientLightFactor, g * ambientLightFactor, b * ambientLightFactor, a, IndexTextures[0], 0.0F, 0.0F, 1.0F, brightness));
		break;
	case BlockFaces::East:
		triangleIndex1 = tessellationHelper.AddVertex(Vertex(BlockVerticesPositions[4], BlockUVs[12], BlockUVs[13], r * ambientLightFactor, g * ambientLightFactor, b * ambientLightFactor, a, IndexTextures[0], 1.0F, 0.0F, 0.0F, brightness));
		triangleIndex2 = tessellationHelper.AddVertex(Vertex(BlockVerticesPositions[6], BlockUVs[12], BlockUVs[15], r * ambientLightFactor, g * ambientLightFactor, b * ambientLightFactor, a, IndexTextures[0], 1.0F, 0.0F, 0.0F, brightness));
		triangleIndex3 = tessellationHelper.AddVertex(Vertex(BlockVerticesPositions[5], BlockUVs[14], BlockUVs[13], r * ambientLightFactor, g * ambientLightFactor, b * ambientLightFactor, a, IndexTextures[0], 1.0F, 0.0F, 0.0F, brightness));
		triangleIndex4 = tessellationHelper.AddVertex(Vertex(BlockVerticesPositions[7], BlockUVs[14], BlockUVs[15], r * ambientLightFactor, g * ambientLightFactor, b * ambientLightFactor, a, IndexTextures[0], 1.0F, 0.0F, 0.0F, brightness));
		break;
	case BlockFaces::West:
		triangleIndex1 = tessellationHelper.AddVertex(Vertex(BlockVerticesPositions[0], BlockUVs[8], BlockUVs[9], r * ambientLightFactor, g * ambientLightFactor, b * ambientLightFactor, a, IndexTextures[0], -1.0F, 0.0F, 0.0F, brightness));
		triangleIndex2 = tessellationHelper.AddVertex(Vertex(BlockVerticesPositions[1], BlockUVs[10], BlockUVs[9], r * ambientLightFactor, g * ambientLightFactor, b * ambientLightFactor, a, IndexTextures[0], -1.0F, 0.0F, 0.0F, brightness));
		triangleIndex3 = tessellationHelper.AddVertex(Vertex(BlockVerticesPositions[2], BlockUVs[8], BlockUVs[11], r * ambientLightFactor, g * ambientLightFactor, b * ambientLightFactor, a, IndexTextures[0], -1.0F, 0.0F, 0.0F, brightness));
		triangleIndex4 = tessellationHelper.AddVertex(Vertex(BlockVerticesPositions[3], BlockUVs[10], BlockUVs[11], r * ambientLightFactor, g * ambientLightFactor, b * ambientLightFactor, a, IndexTextures[0], -1.0F, 0.0F, 0.0F, brightness));
		break;
	}
	tessellationHelper.AddTriangle(triangleIndex1);
	tessellationHelper.AddTriangle(triangleIndex2);
	tessellationHelper.AddTriangle(triangleIndex3);
	tessellationHelper.AddTriangle(triangleIndex4);
	tessellationHelper.AddTriangle(triangleIndex3);
	tessellationHelper.AddTriangle(triangleIndex2);
}

void EntityMeshBlock::PrepareRender(const TessellationHelper& tessellationHelper)
{
	TrianglesStartPos = tessellationHelper.GetCurrentTriangleCount();
	const float minX = BlockBounds.GetMinX();
	const float minY = BlockBounds.GetMinY();
	const float minZ = BlockBounds.GetMinZ();
	const float maxX = BlockBounds.GetMaxX();
	const float maxY = BlockBounds.GetMaxY();
	const float maxZ = BlockBounds.GetMaxZ();
	BlockVerticesPositions[0] = glm::vec3(minX, minY, minZ);
	BlockVerticesPositions[1] = glm::vec3(minX, minY, maxZ);
	BlockVerticesPositions[2] = glm::vec3(minX, maxY, minZ);
	BlockVerticesPositions[3] = glm::vec3(minX, maxY, maxZ);
	BlockVerticesPositions[4] = glm::vec3(maxX, minY, minZ);
	BlockVerticesPositions[5] = glm::vec3(maxX, minY, maxZ);
	BlockVerticesPositions[6] = glm::vec3(maxX, maxY, minZ);
	BlockVerticesPositions[7] = glm::vec3(maxX, maxY, maxZ);
}

void EntityMeshBlock::SetPrevTransform()
{
	PrevTransform = CurrentTransform;
}

size_t EntityMeshBlock::GetBlockTransformId() const
{
	return BlockTransformId;
}

size_t EntityMeshBlock::GetTrianglesStartPos() const
{
	return TrianglesStartPos;
}

size_t EntityMeshBlock::GetIndexTextureSide(BlockFaces /*face*/) const
{
	return 0;
}

TransformStruct& EntityMeshBlock::GetTransform()
{
	return CurrentTransform;
}

const TransformStruct& EntityMeshBlock::GetPrevTransform() const
{
	return PrevTransform;
}
