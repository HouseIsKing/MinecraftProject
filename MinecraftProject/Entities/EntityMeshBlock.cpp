#include "EntityMeshBlock.h"

#include "Util/EngineDefaults.h"

EntityMeshBlock::EntityMeshBlock(const string& meshTexture, const float anchorPosX, const float anchorPosY, const float anchorPosZ, const float minX, const float minY, const float minZ, const float maxX, const float maxY, const float maxZ, const std::array<float, 24>& uvs) : Block(minX, minY, minZ, maxX, maxY, maxZ), BlockUVs(uvs), BlockVerticesPositions()
{
	BlockTransform.SetPosition(anchorPosX, anchorPosY, anchorPosZ);
	IndexTextures.push_back(EngineDefaults::RegisterTexture(Texture::LoadTexture(meshTexture)));
}

void EntityMeshBlock::SetRotation(const float x, const float y, const float z)
{
	BlockTransform.SetRotation(x, y, z);
}

void EntityMeshBlock::SetRotationRadians(const float x, const float y, const float z)
{
	BlockTransform.SetRotation(glm::degrees(x), glm::degrees(y), glm::degrees(z));
}

void EntityMeshBlock::GenerateTessellationData(TessellationHelper& tessellationHelper) const
{
	GenerateTessellationData(tessellationHelper, BlockFaces::Top, 0, 0, 0, 1.0F);
	GenerateTessellationData(tessellationHelper, BlockFaces::Bottom, 0, 0, 0, 1.0F);
	GenerateTessellationData(tessellationHelper, BlockFaces::South, 0, 0, 0, 1.0F);
	GenerateTessellationData(tessellationHelper, BlockFaces::North, 0, 0, 0, 1.0F);
	GenerateTessellationData(tessellationHelper, BlockFaces::East, 0, 0, 0, 1.0F);
	GenerateTessellationData(tessellationHelper, BlockFaces::West, 0, 0, 0, 1.0F);
}

void EntityMeshBlock::GenerateTessellationData(TessellationHelper& tessellationHelper, const BlockFaces& face,
                                               const float& /*x*/, const float& /*y*/, const float& /*z*/, const float& brightness) const
{
	uint16_t triangleIndex1 = 0;
	uint16_t triangleIndex2 = 0;
	uint16_t triangleIndex3 = 0;
	uint16_t triangleIndex4 = 0;
	switch (face)
	{
	case BlockFaces::Top:
		triangleIndex1 = tessellationHelper.AddVertex(Vertex(BlockVerticesPositions[2], BlockUVs[0], BlockUVs[3], 1.0F, 1.0F, 1.0F, 1.0F, IndexTextures[0], 0.0F, 1.0F, 0.0F, brightness));
		triangleIndex2 = tessellationHelper.AddVertex(Vertex(BlockVerticesPositions[3], BlockUVs[0], BlockUVs[1], 1.0F, 1.0F, 1.0F, 1.0F, IndexTextures[0], 0.0F, 1.0F, 0.0F, brightness));
		triangleIndex3 = tessellationHelper.AddVertex(Vertex(BlockVerticesPositions[6], BlockUVs[2], BlockUVs[3], 1.0F, 1.0F, 1.0F, 1.0F, IndexTextures[0], 0.0F, 1.0F, 0.0F, brightness));
		triangleIndex4 = tessellationHelper.AddVertex(Vertex(BlockVerticesPositions[7], BlockUVs[2], BlockUVs[1], 1.0F, 1.0F, 1.0F, 1.0F, IndexTextures[0], 0.0F, 1.0F, 0.0F, brightness));
		break;
	case BlockFaces::Bottom:
		triangleIndex1 = tessellationHelper.AddVertex(Vertex(BlockVerticesPositions[0], BlockUVs[4], BlockUVs[5], 1.0F, 1.0F, 1.0F, 1.0F, IndexTextures[0], 0.0F, -1.0F, 0.0F, brightness));
		triangleIndex2 = tessellationHelper.AddVertex(Vertex(BlockVerticesPositions[4], BlockUVs[6], BlockUVs[5], 1.0F, 1.0F, 1.0F, 1.0F, IndexTextures[0], 0.0F, -1.0F, 0.0F, brightness));
		triangleIndex3 = tessellationHelper.AddVertex(Vertex(BlockVerticesPositions[1], BlockUVs[4], BlockUVs[7], 1.0F, 1.0F, 1.0F, 1.0F, IndexTextures[0], 0.0F, -1.0F, 0.0F, brightness));
		triangleIndex4 = tessellationHelper.AddVertex(Vertex(BlockVerticesPositions[5], BlockUVs[6], BlockUVs[7], 1.0F, 1.0F, 1.0F, 1.0F, IndexTextures[0], 0.0F, -1.0F, 0.0F, brightness));
		break;
	case BlockFaces::South:
		triangleIndex1 = tessellationHelper.AddVertex(Vertex(BlockVerticesPositions[0], BlockUVs[8], BlockUVs[9], 1.0F, 1.0F, 1.0F, 1.0F, IndexTextures[0], 0.0F, 0.0F, -1.0F, brightness));
		triangleIndex2 = tessellationHelper.AddVertex(Vertex(BlockVerticesPositions[2], BlockUVs[8], BlockUVs[11], 1.0F, 1.0F, 1.0F, 1.0F, IndexTextures[0], 0.0F, 0.0F, -1.0F, brightness));
		triangleIndex3 = tessellationHelper.AddVertex(Vertex(BlockVerticesPositions[4], BlockUVs[10], BlockUVs[9], 1.0F, 1.0F, 1.0F, 1.0F, IndexTextures[0], 0.0F, 0.0F, -1.0F, brightness));
		triangleIndex4 = tessellationHelper.AddVertex(Vertex(BlockVerticesPositions[6], BlockUVs[10], BlockUVs[11], 1.0F, 1.0F, 1.0F, 1.0F, IndexTextures[0], 0.0F, 0.0F, -1.0F, brightness));
		break;
	case BlockFaces::North:
		triangleIndex1 = tessellationHelper.AddVertex(Vertex(BlockVerticesPositions[1], BlockUVs[12], BlockUVs[13], 1.0F, 1.0F, 1.0F, 1.0F, IndexTextures[0], 0.0F, 0.0F, 1.0F, brightness));
		triangleIndex2 = tessellationHelper.AddVertex(Vertex(BlockVerticesPositions[5], BlockUVs[14], BlockUVs[13], 1.0F, 1.0F, 1.0F, 1.0F, IndexTextures[0], 0.0F, 0.0F, 1.0F, brightness));
		triangleIndex3 = tessellationHelper.AddVertex(Vertex(BlockVerticesPositions[3], BlockUVs[12], BlockUVs[15], 1.0F, 1.0F, 1.0F, 1.0F, IndexTextures[0], 0.0F, 0.0F, 1.0F, brightness));
		triangleIndex4 = tessellationHelper.AddVertex(Vertex(BlockVerticesPositions[7], BlockUVs[14], BlockUVs[15], 1.0F, 1.0F, 1.0F, 1.0F, IndexTextures[0], 0.0F, 0.0F, 1.0F, brightness));
		break;
	case BlockFaces::East:
		triangleIndex1 = tessellationHelper.AddVertex(Vertex(BlockVerticesPositions[4], BlockUVs[18], BlockUVs[17], 1.0F, 1.0F, 1.0F, 1.0F, IndexTextures[0], 1.0F, 0.0F, 0.0F, brightness));
		triangleIndex2 = tessellationHelper.AddVertex(Vertex(BlockVerticesPositions[6], BlockUVs[18], BlockUVs[19], 1.0F, 1.0F, 1.0F, 1.0F, IndexTextures[0], 1.0F, 0.0F, 0.0F, brightness));
		triangleIndex3 = tessellationHelper.AddVertex(Vertex(BlockVerticesPositions[5], BlockUVs[16], BlockUVs[17], 1.0F, 1.0F, 1.0F, 1.0F, IndexTextures[0], 1.0F, 0.0F, 0.0F, brightness));
		triangleIndex4 = tessellationHelper.AddVertex(Vertex(BlockVerticesPositions[7], BlockUVs[16], BlockUVs[19], 1.0F, 1.0F, 1.0F, 1.0F, IndexTextures[0], 1.0F, 0.0F, 0.0F, brightness));
		break;
	case BlockFaces::West:
		triangleIndex1 = tessellationHelper.AddVertex(Vertex(BlockVerticesPositions[0], BlockUVs[20], BlockUVs[21], 1.0F, 1.0F, 1.0F, 1.0F, IndexTextures[0], -1.0F, 0.0F, 0.0F, brightness));
		triangleIndex2 = tessellationHelper.AddVertex(Vertex(BlockVerticesPositions[1], BlockUVs[22], BlockUVs[21], 1.0F, 1.0F, 1.0F, 1.0F, IndexTextures[0], -1.0F, 0.0F, 0.0F, brightness));
		triangleIndex3 = tessellationHelper.AddVertex(Vertex(BlockVerticesPositions[2], BlockUVs[20], BlockUVs[23], 1.0F, 1.0F, 1.0F, 1.0F, IndexTextures[0], -1.0F, 0.0F, 0.0F, brightness));
		triangleIndex4 = tessellationHelper.AddVertex(Vertex(BlockVerticesPositions[3], BlockUVs[22], BlockUVs[23], 1.0F, 1.0F, 1.0F, 1.0F, IndexTextures[0], -1.0F, 0.0F, 0.0F, brightness));
		break;
	}
	tessellationHelper.AddTriangle(triangleIndex1);
	tessellationHelper.AddTriangle(triangleIndex2);
	tessellationHelper.AddTriangle(triangleIndex3);
	tessellationHelper.AddTriangle(triangleIndex4);
	tessellationHelper.AddTriangle(triangleIndex3);
	tessellationHelper.AddTriangle(triangleIndex2);
}

void EntityMeshBlock::PrepareRender()
{
	const float minX = BlockBounds.GetMinX();
	const float minY = BlockBounds.GetMinY();
	const float minZ = BlockBounds.GetMinZ();
	const float maxX = BlockBounds.GetMaxX();
	const float maxY = BlockBounds.GetMaxY();
	const float maxZ = BlockBounds.GetMaxZ();
	BlockVerticesPositions[0] = vec3(BlockTransform.GetTransformMatrix() * vec4(minX, minY, minZ, 1.0F));
	BlockVerticesPositions[1] = vec3(BlockTransform.GetTransformMatrix() * vec4(minX, minY, maxZ, 1.0F));
	BlockVerticesPositions[2] = vec3(BlockTransform.GetTransformMatrix() * vec4(minX, maxY, minZ, 1.0F));
	BlockVerticesPositions[3] = vec3(BlockTransform.GetTransformMatrix() * vec4(minX, maxY, maxZ, 1.0F));
	BlockVerticesPositions[4] = vec3(BlockTransform.GetTransformMatrix() * vec4(maxX, minY, minZ, 1.0F));
	BlockVerticesPositions[5] = vec3(BlockTransform.GetTransformMatrix() * vec4(maxX, minY, maxZ, 1.0F));
	BlockVerticesPositions[6] = vec3(BlockTransform.GetTransformMatrix() * vec4(maxX, maxY, minZ, 1.0F));
	BlockVerticesPositions[7] = vec3(BlockTransform.GetTransformMatrix() * vec4(maxX, maxY, maxZ, 1.0F));
}
