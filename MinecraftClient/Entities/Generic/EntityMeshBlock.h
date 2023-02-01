#pragma once
#include "World/Generic/Blocks/Block.h"
#include <array>

class EntityMeshBlock final : Block
{
	size_t TrianglesStartPos;
	std::array<float, 24> BlockUVs;
	std::array<glm::vec3, 8> BlockVerticesPositions;
	size_t BlockTransformId;
	TransformStruct CurrentTransform;
	Transform& EntityBlockTransform;
	TransformStruct PrevTransform;

public:
	EntityMeshBlock(const std::string& meshTexture, float anchorPosX, float anchorPosY, float anchorPosZ, float minX, float minY, float minZ, float maxX, float maxY, float maxZ, const std::array<float, 24>& uvs, TessellationHelper& tessellationHelper);
	void GenerateTessellationData(TessellationHelper& tessellationHelper, int brightness) const;
	void GenerateTessellationData(GuiTessellation& tessellationHelper, const BlockFaces& face, glm::mat4x4 transformationMatrix) const override;
	void GenerateTessellationData(TessellationHelper& tessellationHelper, const BlockFaces& face, const float& x, const float& y, const float& z, const int& brightness, float r, float g, float b, float a) const override;
	void PrepareRender(const TessellationHelper& tessellationHelper);
	void SetPrevTransform();
	[[nodiscard]] size_t GetBlockTransformId() const;
	[[nodiscard]] size_t GetTrianglesStartPos() const;
	[[nodiscard]] size_t GetIndexTextureSide(BlockFaces face) const override;
	[[nodiscard]] TransformStruct& GetTransform();
	[[nodiscard]] const TransformStruct& GetPrevTransform() const;
};
