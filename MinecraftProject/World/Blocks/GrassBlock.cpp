//
// Created by amit on 4/22/2022.
//

#include "GrassBlock.h"

void GrassBlock::generateTessellationData(TessellationHelper& tessellationHelper, const BlockFaces& face, const float& x, const float& y, const float& z) const
{
	uint16_t triangleIndex1 = 0;
	uint16_t triangleIndex2 = 0;
	uint16_t triangleIndex3 = 0;
	uint16_t triangleIndex4 = 0;
	switch (face)
	{
	case BlockFaces::TOP:
		triangleIndex1 = tessellationHelper.AddVertex(Vertex(x + boundingBox.getMinX(), y + boundingBox.getMaxY(), z + boundingBox.getMinZ(), 0, 0, 0.368627451F, 0.6156862745F, 0.2039215686F, 1.0F, IndexTextures[0]));
		triangleIndex2 = tessellationHelper.AddVertex(Vertex(x + boundingBox.getMinX(), y + boundingBox.getMaxY(), z + boundingBox.getMaxZ(), 0, 1, 0.368627451F, 0.6156862745F, 0.2039215686F, 1.0F, IndexTextures[0]));
		triangleIndex3 = tessellationHelper.AddVertex(Vertex(x + boundingBox.getMaxX(), y + boundingBox.getMaxY(), z + boundingBox.getMinZ(), 1, 0, 0.368627451F, 0.6156862745F, 0.2039215686F, 1.0F, IndexTextures[0]));
		triangleIndex4 = tessellationHelper.AddVertex(Vertex(x + boundingBox.getMaxX(), y + boundingBox.getMaxY(), z + boundingBox.getMaxZ(), 1, 1, 0.368627451F, 0.6156862745F, 0.2039215686F, 1.0F, IndexTextures[0]));
		break;
	case BlockFaces::BOTTOM:
		triangleIndex1 = tessellationHelper.AddVertex(Vertex(x + boundingBox.getMinX(), y + boundingBox.getMinY(), z + boundingBox.getMinZ(), 0, 0, 1.0F, 1.0F, 1.0F, 1.0F, IndexTextures[2]));
		triangleIndex2 = tessellationHelper.AddVertex(Vertex(x + boundingBox.getMaxX(), y + boundingBox.getMinY(), z + boundingBox.getMinZ(), 1, 0, 1.0F, 1.0F, 1.0F, 1.0F, IndexTextures[2]));
		triangleIndex3 = tessellationHelper.AddVertex(Vertex(x + boundingBox.getMinX(), y + boundingBox.getMinY(), z + boundingBox.getMaxZ(), 0, 1, 1.0F, 1.0F, 1.0F, 1.0F, IndexTextures[2]));
		triangleIndex4 = tessellationHelper.AddVertex(Vertex(x + boundingBox.getMaxX(), y + boundingBox.getMinY(), z + boundingBox.getMaxZ(), 1, 1, 1.0F, 1.0F, 1.0F, 1.0F, IndexTextures[2]));
		break;
	case BlockFaces::SOUTH:
		triangleIndex1 = tessellationHelper.AddVertex(Vertex(x + boundingBox.getMinX(), y + boundingBox.getMinY(), z + boundingBox.getMinZ(), 0, 0, 1.0F, 1.0F, 1.0F, 1.0F, IndexTextures[1]));
		triangleIndex2 = tessellationHelper.AddVertex(Vertex(x + boundingBox.getMinX(), y + boundingBox.getMaxY(), z + boundingBox.getMinZ(), 0, 1, 1.0F, 1.0F, 1.0F, 1.0F, IndexTextures[1]));
		triangleIndex3 = tessellationHelper.AddVertex(Vertex(x + boundingBox.getMaxX(), y + boundingBox.getMinY(), z + boundingBox.getMinZ(), 1, 0, 1.0F, 1.0F, 1.0F, 1.0F, IndexTextures[1]));
		triangleIndex4 = tessellationHelper.AddVertex(Vertex(x + boundingBox.getMaxX(), y + boundingBox.getMaxY(), z + boundingBox.getMinZ(), 1, 1, 1.0F, 1.0F, 1.0F, 1.0F, IndexTextures[1]));
		break;
	case BlockFaces::NORTH:
		triangleIndex1 = tessellationHelper.AddVertex(Vertex(x + boundingBox.getMinX(), y + boundingBox.getMinY(), z + boundingBox.getMaxZ(), 0, 0, 1.0F, 1.0F, 1.0F, 1.0F, IndexTextures[1]));
		triangleIndex2 = tessellationHelper.AddVertex(Vertex(x + boundingBox.getMaxX(), y + boundingBox.getMinY(), z + boundingBox.getMaxZ(), 1, 0, 1.0F, 1.0F, 1.0F, 1.0F, IndexTextures[1]));
		triangleIndex3 = tessellationHelper.AddVertex(Vertex(x + boundingBox.getMinX(), y + boundingBox.getMaxY(), z + boundingBox.getMaxZ(), 0, 1, 1.0F, 1.0F, 1.0F, 1.0F, IndexTextures[1]));
		triangleIndex4 = tessellationHelper.AddVertex(Vertex(x + boundingBox.getMaxX(), y + boundingBox.getMaxY(), z + boundingBox.getMaxZ(), 1, 1, 1.0F, 1.0F, 1.0F, 1.0F, IndexTextures[1]));
		break;
	case BlockFaces::EAST:
		triangleIndex1 = tessellationHelper.AddVertex(Vertex(x + boundingBox.getMaxX(), y + boundingBox.getMinY(), z + boundingBox.getMinZ(), 0, 0, 1.0F, 1.0F, 1.0F, 1.0F, IndexTextures[1]));
		triangleIndex2 = tessellationHelper.AddVertex(Vertex(x + boundingBox.getMaxX(), y + boundingBox.getMaxY(), z + boundingBox.getMinZ(), 0, 1, 1.0F, 1.0F, 1.0F, 1.0F, IndexTextures[1]));
		triangleIndex3 = tessellationHelper.AddVertex(Vertex(x + boundingBox.getMaxX(), y + boundingBox.getMinY(), z + boundingBox.getMaxZ(), 1, 0, 1.0F, 1.0F, 1.0F, 1.0F, IndexTextures[1]));
		triangleIndex4 = tessellationHelper.AddVertex(Vertex(x + boundingBox.getMaxX(), y + boundingBox.getMaxY(), z + boundingBox.getMaxZ(), 1, 1, 1.0F, 1.0F, 1.0F, 1.0F, IndexTextures[1]));
		break;
	case BlockFaces::WEST:
		triangleIndex1 = tessellationHelper.AddVertex(Vertex(x + boundingBox.getMinX(), y + boundingBox.getMinY(), z + boundingBox.getMinZ(), 0, 0, 1.0F, 1.0F, 1.0F, 1.0F, IndexTextures[1]));
		triangleIndex2 = tessellationHelper.AddVertex(Vertex(x + boundingBox.getMinX(), y + boundingBox.getMinY(), z + boundingBox.getMaxZ(), 1, 0, 1.0F, 1.0F, 1.0F, 1.0F, IndexTextures[1]));
		triangleIndex3 = tessellationHelper.AddVertex(Vertex(x + boundingBox.getMinX(), y + boundingBox.getMaxY(), z + boundingBox.getMinZ(), 0, 1, 1.0F, 1.0F, 1.0F, 1.0F, IndexTextures[1]));
		triangleIndex4 = tessellationHelper.AddVertex(Vertex(x + boundingBox.getMinX(), y + boundingBox.getMaxY(), z + boundingBox.getMaxZ(), 1, 1, 1.0F, 1.0F, 1.0F, 1.0F, IndexTextures[1]));
		break;
	}
	tessellationHelper.AddTriangle(triangleIndex1);
	tessellationHelper.AddTriangle(triangleIndex2);
	tessellationHelper.AddTriangle(triangleIndex3);
	tessellationHelper.AddTriangle(triangleIndex4);
	tessellationHelper.AddTriangle(triangleIndex3);
	tessellationHelper.AddTriangle(triangleIndex2);
}

GrassBlock::GrassBlock()
{
	IndexTextures.push_back(BlockTypeList::RegisterTexture(Texture::LoadTexture("Textures/Blocks/GrassTop.png")));
	IndexTextures.push_back(BlockTypeList::RegisterTexture(Texture::LoadTexture("Textures/Blocks/GrassSide.png")));
	IndexTextures.push_back(BlockTypeList::RegisterTexture(Texture::LoadTexture("Textures/Blocks/Dirt.png")));
}
