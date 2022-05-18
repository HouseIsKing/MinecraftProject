//
// Created by amit on 4/22/2022.
//

#include "GrassBlock.h"

#include "Util/EngineDefaults.h"

void GrassBlock::GenerateTessellationData(TessellationHelper& tessellationHelper, const BlockFaces& face, const float& x, const float& y, const float& z, const float& brightness) const
{
	uint16_t triangleIndex1 = 0;
	uint16_t triangleIndex2 = 0;
	uint16_t triangleIndex3 = 0;
	uint16_t triangleIndex4 = 0;
	switch (face)
	{
	case BlockFaces::Top:
		triangleIndex1 = tessellationHelper.AddVertex(Vertex(x + BlockBounds.GetMinX(), y + BlockBounds.GetMaxY(), z + BlockBounds.GetMinZ(), 0, 0, 0.368627451F, 0.6156862745F, 0.2039215686F, 1.0F, IndexTextures[0], 0.0F, 1.0F, 0.0F, brightness));
		triangleIndex2 = tessellationHelper.AddVertex(Vertex(x + BlockBounds.GetMinX(), y + BlockBounds.GetMaxY(), z + BlockBounds.GetMaxZ(), 0, 1, 0.368627451F, 0.6156862745F, 0.2039215686F, 1.0F, IndexTextures[0], 0.0F, 1.0F, 0.0F, brightness));
		triangleIndex3 = tessellationHelper.AddVertex(Vertex(x + BlockBounds.GetMaxX(), y + BlockBounds.GetMaxY(), z + BlockBounds.GetMinZ(), 1, 0, 0.368627451F, 0.6156862745F, 0.2039215686F, 1.0F, IndexTextures[0], 0.0F, 1.0F, 0.0F, brightness));
		triangleIndex4 = tessellationHelper.AddVertex(Vertex(x + BlockBounds.GetMaxX(), y + BlockBounds.GetMaxY(), z + BlockBounds.GetMaxZ(), 1, 1, 0.368627451F, 0.6156862745F, 0.2039215686F, 1.0F, IndexTextures[0], 0.0F, 1.0F, 0.0F, brightness));
		break;
	case BlockFaces::Bottom:
		triangleIndex1 = tessellationHelper.AddVertex(Vertex(x + BlockBounds.GetMinX(), y + BlockBounds.GetMinY(), z + BlockBounds.GetMinZ(), 0, 0, 1.0F, 1.0F, 1.0F, 1.0F, IndexTextures[2], 0.0F, -1.0F, 0.0F, brightness));
		triangleIndex2 = tessellationHelper.AddVertex(Vertex(x + BlockBounds.GetMaxX(), y + BlockBounds.GetMinY(), z + BlockBounds.GetMinZ(), 1, 0, 1.0F, 1.0F, 1.0F, 1.0F, IndexTextures[2], 0.0F, -1.0F, 0.0F, brightness));
		triangleIndex3 = tessellationHelper.AddVertex(Vertex(x + BlockBounds.GetMinX(), y + BlockBounds.GetMinY(), z + BlockBounds.GetMaxZ(), 0, 1, 1.0F, 1.0F, 1.0F, 1.0F, IndexTextures[2], 0.0F, -1.0F, 0.0F, brightness));
		triangleIndex4 = tessellationHelper.AddVertex(Vertex(x + BlockBounds.GetMaxX(), y + BlockBounds.GetMinY(), z + BlockBounds.GetMaxZ(), 1, 1, 1.0F, 1.0F, 1.0F, 1.0F, IndexTextures[2], 0.0F, -1.0F, 0.0F, brightness));
		break;
	case BlockFaces::South:
		triangleIndex1 = tessellationHelper.AddVertex(Vertex(x + BlockBounds.GetMinX(), y + BlockBounds.GetMinY(), z + BlockBounds.GetMinZ(), 0, 0, 1.0F, 1.0F, 1.0F, 1.0F, IndexTextures[1], 0.0F, 0.0F, -1.0F, brightness));
		triangleIndex2 = tessellationHelper.AddVertex(Vertex(x + BlockBounds.GetMinX(), y + BlockBounds.GetMaxY(), z + BlockBounds.GetMinZ(), 0, 1, 1.0F, 1.0F, 1.0F, 1.0F, IndexTextures[1], 0.0F, 0.0F, -1.0F, brightness));
		triangleIndex3 = tessellationHelper.AddVertex(Vertex(x + BlockBounds.GetMaxX(), y + BlockBounds.GetMinY(), z + BlockBounds.GetMinZ(), 1, 0, 1.0F, 1.0F, 1.0F, 1.0F, IndexTextures[1], 0.0F, 0.0F, -1.0F, brightness));
		triangleIndex4 = tessellationHelper.AddVertex(Vertex(x + BlockBounds.GetMaxX(), y + BlockBounds.GetMaxY(), z + BlockBounds.GetMinZ(), 1, 1, 1.0F, 1.0F, 1.0F, 1.0F, IndexTextures[1], 0.0F, 0.0F, -1.0F, brightness));
		break;
	case BlockFaces::North:
		triangleIndex1 = tessellationHelper.AddVertex(Vertex(x + BlockBounds.GetMinX(), y + BlockBounds.GetMinY(), z + BlockBounds.GetMaxZ(), 0, 0, 1.0F, 1.0F, 1.0F, 1.0F, IndexTextures[1], 0.0F, 0.0F, 1.0F, brightness));
		triangleIndex2 = tessellationHelper.AddVertex(Vertex(x + BlockBounds.GetMaxX(), y + BlockBounds.GetMinY(), z + BlockBounds.GetMaxZ(), 1, 0, 1.0F, 1.0F, 1.0F, 1.0F, IndexTextures[1], 0.0F, 0.0F, 1.0F, brightness));
		triangleIndex3 = tessellationHelper.AddVertex(Vertex(x + BlockBounds.GetMinX(), y + BlockBounds.GetMaxY(), z + BlockBounds.GetMaxZ(), 0, 1, 1.0F, 1.0F, 1.0F, 1.0F, IndexTextures[1], 0.0F, 0.0F, 1.0F, brightness));
		triangleIndex4 = tessellationHelper.AddVertex(Vertex(x + BlockBounds.GetMaxX(), y + BlockBounds.GetMaxY(), z + BlockBounds.GetMaxZ(), 1, 1, 1.0F, 1.0F, 1.0F, 1.0F, IndexTextures[1], 0.0F, 0.0F, 1.0F, brightness));
		break;
	case BlockFaces::East:
		triangleIndex1 = tessellationHelper.AddVertex(Vertex(x + BlockBounds.GetMaxX(), y + BlockBounds.GetMinY(), z + BlockBounds.GetMinZ(), 0, 0, 1.0F, 1.0F, 1.0F, 1.0F, IndexTextures[1], 1.0F, 0.0F, 0.0F, brightness));
		triangleIndex2 = tessellationHelper.AddVertex(Vertex(x + BlockBounds.GetMaxX(), y + BlockBounds.GetMaxY(), z + BlockBounds.GetMinZ(), 0, 1, 1.0F, 1.0F, 1.0F, 1.0F, IndexTextures[1], 1.0F, 0.0F, 0.0F, brightness));
		triangleIndex3 = tessellationHelper.AddVertex(Vertex(x + BlockBounds.GetMaxX(), y + BlockBounds.GetMinY(), z + BlockBounds.GetMaxZ(), 1, 0, 1.0F, 1.0F, 1.0F, 1.0F, IndexTextures[1], 1.0F, 0.0F, 0.0F, brightness));
		triangleIndex4 = tessellationHelper.AddVertex(Vertex(x + BlockBounds.GetMaxX(), y + BlockBounds.GetMaxY(), z + BlockBounds.GetMaxZ(), 1, 1, 1.0F, 1.0F, 1.0F, 1.0F, IndexTextures[1], 1.0F, 0.0F, 0.0F, brightness));
		break;
	case BlockFaces::West:
		triangleIndex1 = tessellationHelper.AddVertex(Vertex(x + BlockBounds.GetMinX(), y + BlockBounds.GetMinY(), z + BlockBounds.GetMinZ(), 0, 0, 1.0F, 1.0F, 1.0F, 1.0F, IndexTextures[1], -1.0F, 0.0F, 0.0F, brightness));
		triangleIndex2 = tessellationHelper.AddVertex(Vertex(x + BlockBounds.GetMinX(), y + BlockBounds.GetMinY(), z + BlockBounds.GetMaxZ(), 1, 0, 1.0F, 1.0F, 1.0F, 1.0F, IndexTextures[1], -1.0F, 0.0F, 0.0F, brightness));
		triangleIndex3 = tessellationHelper.AddVertex(Vertex(x + BlockBounds.GetMinX(), y + BlockBounds.GetMaxY(), z + BlockBounds.GetMinZ(), 0, 1, 1.0F, 1.0F, 1.0F, 1.0F, IndexTextures[1], -1.0F, 0.0F, 0.0F, brightness));
		triangleIndex4 = tessellationHelper.AddVertex(Vertex(x + BlockBounds.GetMinX(), y + BlockBounds.GetMaxY(), z + BlockBounds.GetMaxZ(), 1, 1, 1.0F, 1.0F, 1.0F, 1.0F, IndexTextures[1], -1.0F, 0.0F, 0.0F, brightness));
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
	IndexTextures.push_back(EngineDefaults::RegisterTexture(Texture::LoadTexture("Textures/Blocks/GrassTop.png")));
	IndexTextures.push_back(EngineDefaults::RegisterTexture(Texture::LoadTexture("Textures/Blocks/GrassSide.png")));
	IndexTextures.push_back(EngineDefaults::RegisterTexture(Texture::LoadTexture("Textures/Blocks/Dirt.png")));
}
