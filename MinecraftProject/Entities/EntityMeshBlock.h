#pragma once
#include "World/Blocks/Block.h"

class EntityMeshBlock final : Block
{
    Transform BlockTransform;
    std::array<float, 24> BlockUVs;
    std::array<vec3, 8> BlockVerticesPositions;
public:
    EntityMeshBlock(const string& meshTexture, float anchorPosX, float anchorPosY, float anchorPosZ, float minX, float minY, float minZ, float maxX, float maxY, float maxZ, const std::array<float, 24>& uvs);
    void SetRotation(float x, float y, float z);
    void SetRotationRadians(float x, float y, float z);
    void GenerateTessellationData(TessellationHelper& tessellationHelper) const;
    void GenerateTessellationData(TessellationHelper& tessellationHelper, const BlockFaces& face, const float& x, const float& y, const float& z, const float& brightness) const override;
    void PrepareRender();
};
