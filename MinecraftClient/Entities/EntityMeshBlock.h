#pragma once
#include "World/Blocks/Block.h"

class EntityMeshBlock final : Block
{
	size_t BlockTransformId;
	size_t TrianglesStartPos;
	std::array<float, 24> BlockUVs;
	std::array<vec3, 8> BlockVerticesPositions;

public:
	EntityMeshBlock(const string& meshTexture, float anchorPosX, float anchorPosY, float anchorPosZ, float minX, float minY, float minZ, float maxX, float maxY, float maxZ, const std::array<float, 24>& uvs, TessellationHelper& tessellationHelper);
	void SetRotation(float x, float y, float z, const TessellationHelper& tessellationHelper) const;
	void SetRotationRadians(float x, float y, float z, const TessellationHelper& tessellationHelper) const;
	void GenerateTessellationData(TessellationHelper& tessellationHelper, int brightness) const;
	void GenerateTessellationData(GuiTessellation& tessellationHelper, const BlockFaces& face, mat4x4 transformationMatrix) const override;
	void GenerateTessellationData(TessellationHelper& tessellationHelper, const BlockFaces& face, const float& x, const float& y, const float& z, const int& brightness, float r, float g, float b, float a) const override;
	void PrepareRender(const TessellationHelper& tessellationHelper);
	[[nodiscard]] size_t GetBlockTransformId() const;
	[[nodiscard]] size_t GetTrianglesStartPos() const;
	[[nodiscard]] size_t GetIndexTextureSide(BlockFaces face) const override;
};