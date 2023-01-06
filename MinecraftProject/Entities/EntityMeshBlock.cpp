#include "EntityMeshBlock.h"
#include "Util/EngineDefaults.h"

EntityMeshBlock::EntityMeshBlock(const string& meshTexture, const float anchorPosX, const float anchorPosY, const float anchorPosZ, const float minX, const float minY, const float minZ, const float maxX, const float maxY, const float maxZ, const std::array<float, 24>& uvs, TessellationHelper& tessellationHelper) : Block(minX, minY, minZ, maxX, maxY, maxZ),
	BlockTransformId(tessellationHelper.AddTransform(new Transform(tessellationHelper.GetTransform(0)))), TrianglesStartPos(0), BlockUVs(uvs), BlockVerticesPositions()
{
	tessellationHelper.GetTransform(BlockTransformId)->SetPosition(anchorPosX, anchorPosY, anchorPosZ);
	IndexTextures.push_back(EngineDefaults::RegisterTexture(Texture::LoadTexture(meshTexture)));
}

void EntityMeshBlock::SetRotation(const float x, const float y, const float z, const TessellationHelper& tessellationHelper) const
{
	tessellationHelper.GetTransform(BlockTransformId)->SetRotation(x, y, z);
}

void EntityMeshBlock::SetRotationRadians(const float x, const float y, const float z, const TessellationHelper& tessellationHelper) const
{
	tessellationHelper.GetTransform(BlockTransformId)->SetRotation(glm::degrees(x), glm::degrees(y), glm::degrees(z));
}

void EntityMeshBlock::GenerateTessellationData(TessellationHelper& tessellationHelper, const int brightness) const
{
	GenerateTessellationData(tessellationHelper, BlockFaces::Top, 0, 0, 0, brightness);
	GenerateTessellationData(tessellationHelper, BlockFaces::Bottom, 0, 0, 0, brightness);
	GenerateTessellationData(tessellationHelper, BlockFaces::South, 0, 0, 0, brightness);
	GenerateTessellationData(tessellationHelper, BlockFaces::North, 0, 0, 0, brightness);
	GenerateTessellationData(tessellationHelper, BlockFaces::East, 0, 0, 0, brightness);
	GenerateTessellationData(tessellationHelper, BlockFaces::West, 0, 0, 0, brightness);
}

void EntityMeshBlock::GenerateTessellationData(GuiTessellation& /*tessellationHelper*/, const BlockFaces& /*face*/, mat4x4 /*transformationMatrix*/) const
{
}

void EntityMeshBlock::GenerateTessellationData(TessellationHelper& tessellationHelper, const BlockFaces& face,
                                               const float& /*x*/, const float& /*y*/, const float& /*z*/, const int& brightness) const
{
	uint16_t triangleIndex1 = 0;
	uint16_t triangleIndex2 = 0;
	uint16_t triangleIndex3 = 0;
	uint16_t triangleIndex4 = 0;
	const float ambientLightFactor = EngineDefaults::ConvertLightLevelToAmbient(brightness);
	switch (face)
	{
	case BlockFaces::Top:
		triangleIndex1 = tessellationHelper.AddVertex(Vertex(BlockVerticesPositions[2], BlockUVs[0], BlockUVs[3], ambientLightFactor, ambientLightFactor, ambientLightFactor, 1.0F, IndexTextures[0], 0.0F, 1.0F, 0.0F, brightness));
		triangleIndex2 = tessellationHelper.AddVertex(Vertex(BlockVerticesPositions[3], BlockUVs[2], BlockUVs[3], ambientLightFactor, ambientLightFactor, ambientLightFactor, 1.0F, IndexTextures[0], 0.0F, 1.0F, 0.0F, brightness));
		triangleIndex3 = tessellationHelper.AddVertex(Vertex(BlockVerticesPositions[6], BlockUVs[0], BlockUVs[1], ambientLightFactor, ambientLightFactor, ambientLightFactor, 1.0F, IndexTextures[0], 0.0F, 1.0F, 0.0F, brightness));
		triangleIndex4 = tessellationHelper.AddVertex(Vertex(BlockVerticesPositions[7], BlockUVs[2], BlockUVs[1], ambientLightFactor, ambientLightFactor, ambientLightFactor, 1.0F, IndexTextures[0], 0.0F, 1.0F, 0.0F, brightness));
		break;
	case BlockFaces::Bottom:
		triangleIndex1 = tessellationHelper.AddVertex(Vertex(BlockVerticesPositions[0], BlockUVs[4], BlockUVs[5], ambientLightFactor, ambientLightFactor, ambientLightFactor, 1.0F, IndexTextures[0], 0.0F, -1.0F, 0.0F, brightness));
		triangleIndex2 = tessellationHelper.AddVertex(Vertex(BlockVerticesPositions[4], BlockUVs[4], BlockUVs[7], ambientLightFactor, ambientLightFactor, ambientLightFactor, 1.0F, IndexTextures[0], 0.0F, -1.0F, 0.0F, brightness));
		triangleIndex3 = tessellationHelper.AddVertex(Vertex(BlockVerticesPositions[1], BlockUVs[6], BlockUVs[5], ambientLightFactor, ambientLightFactor, ambientLightFactor, 1.0F, IndexTextures[0], 0.0F, -1.0F, 0.0F, brightness));
		triangleIndex4 = tessellationHelper.AddVertex(Vertex(BlockVerticesPositions[5], BlockUVs[6], BlockUVs[7], ambientLightFactor, ambientLightFactor, ambientLightFactor, 1.0F, IndexTextures[0], 0.0F, -1.0F, 0.0F, brightness));
		break;
	case BlockFaces::South:
		triangleIndex1 = tessellationHelper.AddVertex(Vertex(BlockVerticesPositions[0], BlockUVs[20], BlockUVs[21], ambientLightFactor, ambientLightFactor, ambientLightFactor, 1.0F, IndexTextures[0], 0.0F, 0.0F, -1.0F, brightness));
		triangleIndex2 = tessellationHelper.AddVertex(Vertex(BlockVerticesPositions[2], BlockUVs[20], BlockUVs[23], ambientLightFactor, ambientLightFactor, ambientLightFactor, 1.0F, IndexTextures[0], 0.0F, 0.0F, -1.0F, brightness));
		triangleIndex3 = tessellationHelper.AddVertex(Vertex(BlockVerticesPositions[4], BlockUVs[22], BlockUVs[21], ambientLightFactor, ambientLightFactor, ambientLightFactor, 1.0F, IndexTextures[0], 0.0F, 0.0F, -1.0F, brightness));
		triangleIndex4 = tessellationHelper.AddVertex(Vertex(BlockVerticesPositions[6], BlockUVs[22], BlockUVs[23], ambientLightFactor, ambientLightFactor, ambientLightFactor, 1.0F, IndexTextures[0], 0.0F, 0.0F, -1.0F, brightness));
		break;
	case BlockFaces::North:
		triangleIndex1 = tessellationHelper.AddVertex(Vertex(BlockVerticesPositions[1], BlockUVs[18], BlockUVs[17], ambientLightFactor, ambientLightFactor, ambientLightFactor, 1.0F, IndexTextures[0], 0.0F, 0.0F, 1.0F, brightness));
		triangleIndex2 = tessellationHelper.AddVertex(Vertex(BlockVerticesPositions[5], BlockUVs[16], BlockUVs[17], ambientLightFactor, ambientLightFactor, ambientLightFactor, 1.0F, IndexTextures[0], 0.0F, 0.0F, 1.0F, brightness));
		triangleIndex3 = tessellationHelper.AddVertex(Vertex(BlockVerticesPositions[3], BlockUVs[18], BlockUVs[19], ambientLightFactor, ambientLightFactor, ambientLightFactor, 1.0F, IndexTextures[0], 0.0F, 0.0F, 1.0F, brightness));
		triangleIndex4 = tessellationHelper.AddVertex(Vertex(BlockVerticesPositions[7], BlockUVs[16], BlockUVs[19], ambientLightFactor, ambientLightFactor, ambientLightFactor, 1.0F, IndexTextures[0], 0.0F, 0.0F, 1.0F, brightness));
		break;
	case BlockFaces::East:
		triangleIndex1 = tessellationHelper.AddVertex(Vertex(BlockVerticesPositions[4], BlockUVs[12], BlockUVs[13], ambientLightFactor, ambientLightFactor, ambientLightFactor, 1.0F, IndexTextures[0], 1.0F, 0.0F, 0.0F, brightness));
		triangleIndex2 = tessellationHelper.AddVertex(Vertex(BlockVerticesPositions[6], BlockUVs[12], BlockUVs[15], ambientLightFactor, ambientLightFactor, ambientLightFactor, 1.0F, IndexTextures[0], 1.0F, 0.0F, 0.0F, brightness));
		triangleIndex3 = tessellationHelper.AddVertex(Vertex(BlockVerticesPositions[5], BlockUVs[14], BlockUVs[13], ambientLightFactor, ambientLightFactor, ambientLightFactor, 1.0F, IndexTextures[0], 1.0F, 0.0F, 0.0F, brightness));
		triangleIndex4 = tessellationHelper.AddVertex(Vertex(BlockVerticesPositions[7], BlockUVs[14], BlockUVs[15], ambientLightFactor, ambientLightFactor, ambientLightFactor, 1.0F, IndexTextures[0], 1.0F, 0.0F, 0.0F, brightness));
		break;
	case BlockFaces::West:
		triangleIndex1 = tessellationHelper.AddVertex(Vertex(BlockVerticesPositions[0], BlockUVs[8], BlockUVs[9], ambientLightFactor, ambientLightFactor, ambientLightFactor, 1.0F, IndexTextures[0], -1.0F, 0.0F, 0.0F, brightness));
		triangleIndex2 = tessellationHelper.AddVertex(Vertex(BlockVerticesPositions[1], BlockUVs[10], BlockUVs[9], ambientLightFactor, ambientLightFactor, ambientLightFactor, 1.0F, IndexTextures[0], -1.0F, 0.0F, 0.0F, brightness));
		triangleIndex3 = tessellationHelper.AddVertex(Vertex(BlockVerticesPositions[2], BlockUVs[8], BlockUVs[11], ambientLightFactor, ambientLightFactor, ambientLightFactor, 1.0F, IndexTextures[0], -1.0F, 0.0F, 0.0F, brightness));
		triangleIndex4 = tessellationHelper.AddVertex(Vertex(BlockVerticesPositions[3], BlockUVs[10], BlockUVs[11], ambientLightFactor, ambientLightFactor, ambientLightFactor, 1.0F, IndexTextures[0], -1.0F, 0.0F, 0.0F, brightness));
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
	BlockVerticesPositions[0] = vec3(minX, minY, minZ);
	BlockVerticesPositions[1] = vec3(minX, minY, maxZ);
	BlockVerticesPositions[2] = vec3(minX, maxY, minZ);
	BlockVerticesPositions[3] = vec3(minX, maxY, maxZ);
	BlockVerticesPositions[4] = vec3(maxX, minY, minZ);
	BlockVerticesPositions[5] = vec3(maxX, minY, maxZ);
	BlockVerticesPositions[6] = vec3(maxX, maxY, minZ);
	BlockVerticesPositions[7] = vec3(maxX, maxY, maxZ);
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
