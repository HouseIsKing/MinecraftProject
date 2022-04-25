//
// Created by amit on 4/22/2022.
//

#include "GrassBlock.h"

#include <memory>

void GrassBlock::generateTessellationData(TessellationHelper& tessellationHelper, const BlockFaces& face, const float& x, const float& y, const float& z) const
{
	Texture* textureToUse = nullptr;
	uint16_t triangleIndex1 = 0, triangleIndex2 = 0, triangleIndex3 = 0, triangleIndex4 = 0;
    switch (face)
    {
    case BlockFaces::TOP:
		textureToUse = textures.at(0);
        triangleIndex1 = tessellationHelper.addVertex(Vertex(x + boundingBox.getMinX(), y + boundingBox.getMaxY(), z + boundingBox.getMinZ(), 0, 0, 0.368627451f, 0.6156862745f, 0.2039215686f,1.0f));
		triangleIndex2 = tessellationHelper.addVertex(Vertex(x + boundingBox.getMinX(), y + boundingBox.getMaxY(), z + boundingBox.getMaxZ(), 0, 1, 0.368627451f, 0.6156862745f, 0.2039215686f,1.0f));
		triangleIndex3 = tessellationHelper.addVertex(Vertex(x + boundingBox.getMaxX(), y + boundingBox.getMaxY(), z + boundingBox.getMinZ(), 1, 0, 0.368627451f, 0.6156862745f, 0.2039215686f,1.0f));
		triangleIndex4 = tessellationHelper.addVertex(Vertex(x + boundingBox.getMaxX(), y + boundingBox.getMaxY(), z + boundingBox.getMaxZ(), 1, 1, 0.368627451f, 0.6156862745f, 0.2039215686f,1.0f));
		break;
    case BlockFaces::BOTTOM:
		textureToUse = textures.at(2);
		triangleIndex1 = tessellationHelper.addVertex(Vertex(x + boundingBox.getMinX(), y + boundingBox.getMinY(), z + boundingBox.getMinZ(), 0, 0, 1.0f,1.0f,1.0f,1.0f));
		triangleIndex2 = tessellationHelper.addVertex(Vertex(x + boundingBox.getMaxX(), y + boundingBox.getMinY(), z + boundingBox.getMinZ(), 1, 0, 1.0f,1.0f,1.0f,1.0f));
		triangleIndex3 = tessellationHelper.addVertex(Vertex(x + boundingBox.getMinX(), y + boundingBox.getMinY(), z + boundingBox.getMaxZ(), 0, 1, 1.0f,1.0f,1.0f,1.0f));
		triangleIndex4 = tessellationHelper.addVertex(Vertex(x + boundingBox.getMaxX(), y + boundingBox.getMinY(), z + boundingBox.getMaxZ(), 1, 1, 1.0f,1.0f,1.0f,1.0f));
		break;
	case BlockFaces::SOUTH:
		textureToUse = textures.at(1);
		triangleIndex1 = tessellationHelper.addVertex(Vertex(x + boundingBox.getMinX(), y + boundingBox.getMinY(), z + boundingBox.getMinZ(), 0, 0, 1.0f,1.0f,1.0f,1.0f));
		triangleIndex2 = tessellationHelper.addVertex(Vertex(x + boundingBox.getMinX(), y + boundingBox.getMaxY(), z + boundingBox.getMinZ(), 0, 1, 1.0f,1.0f,1.0f,1.0f));
		triangleIndex3 = tessellationHelper.addVertex(Vertex(x + boundingBox.getMaxX(), y + boundingBox.getMinY(), z + boundingBox.getMinZ(), 1, 0, 1.0f,1.0f,1.0f,1.0f));
		triangleIndex4 = tessellationHelper.addVertex(Vertex(x + boundingBox.getMaxX(), y + boundingBox.getMaxY(), z + boundingBox.getMinZ(), 1, 1, 1.0f,1.0f,1.0f,1.0f));
		break;
	case BlockFaces::NORTH:
		textureToUse = textures.at(1);
		triangleIndex1 = tessellationHelper.addVertex(Vertex(x + boundingBox.getMinX(), y + boundingBox.getMinY(), z + boundingBox.getMaxZ(), 0, 0, 1.0f, 1.0f, 1.0f, 1.0f));
		triangleIndex2 = tessellationHelper.addVertex(Vertex(x + boundingBox.getMaxX(), y + boundingBox.getMinY(), z + boundingBox.getMaxZ(), 1, 0, 1.0f, 1.0f, 1.0f, 1.0f));
		triangleIndex3 = tessellationHelper.addVertex(Vertex(x + boundingBox.getMinX(), y + boundingBox.getMaxY(), z + boundingBox.getMaxZ(), 0, 1, 1.0f, 1.0f, 1.0f, 1.0f));
		triangleIndex4 = tessellationHelper.addVertex(Vertex(x + boundingBox.getMaxX(), y + boundingBox.getMaxY(), z + boundingBox.getMaxZ(), 1, 1, 1.0f, 1.0f, 1.0f, 1.0f));
		break;
	case BlockFaces::EAST:
		textureToUse = textures.at(1);
		triangleIndex1 = tessellationHelper.addVertex(Vertex(x + boundingBox.getMaxX(), y + boundingBox.getMinY(), z + boundingBox.getMinZ(), 0, 0, 1.0f, 1.0f, 1.0f, 1.0f));
		triangleIndex2 = tessellationHelper.addVertex(Vertex(x + boundingBox.getMaxX(), y + boundingBox.getMaxY(), z + boundingBox.getMinZ(), 0, 1, 1.0f, 1.0f, 1.0f, 1.0f));
		triangleIndex3 = tessellationHelper.addVertex(Vertex(x + boundingBox.getMaxX(), y + boundingBox.getMinY(), z + boundingBox.getMaxZ(), 1, 0, 1.0f, 1.0f, 1.0f, 1.0f));
		triangleIndex4 = tessellationHelper.addVertex(Vertex(x + boundingBox.getMaxX(), y + boundingBox.getMaxY(), z + boundingBox.getMaxZ(), 1, 1, 1.0f, 1.0f, 1.0f, 1.0f));
		break;
	case BlockFaces::WEST:
		textureToUse = textures.at(1);
		triangleIndex1 = tessellationHelper.addVertex(Vertex(x + boundingBox.getMinX(), y + boundingBox.getMinY(), z + boundingBox.getMinZ(), 0, 0, 1.0f, 1.0f, 1.0f, 1.0f));
		triangleIndex2 = tessellationHelper.addVertex(Vertex(x + boundingBox.getMinX(), y + boundingBox.getMinY(), z + boundingBox.getMaxZ(), 1, 0, 1.0f, 1.0f, 1.0f, 1.0f));
		triangleIndex3 = tessellationHelper.addVertex(Vertex(x + boundingBox.getMinX(), y + boundingBox.getMaxY(), z + boundingBox.getMinZ(), 0, 1, 1.0f, 1.0f, 1.0f, 1.0f));
		triangleIndex4 = tessellationHelper.addVertex(Vertex(x + boundingBox.getMinX(), y + boundingBox.getMaxY(), z + boundingBox.getMaxZ(), 1, 1, 1.0f, 1.0f, 1.0f, 1.0f));
		break;
    }
	tessellationHelper.addTriangle(textureToUse, triangleIndex1);
	tessellationHelper.addTriangle(textureToUse, triangleIndex2);
	tessellationHelper.addTriangle(textureToUse, triangleIndex3);
	tessellationHelper.addTriangle(textureToUse, triangleIndex4);
	tessellationHelper.addTriangle(textureToUse, triangleIndex3);
	tessellationHelper.addTriangle(textureToUse, triangleIndex2);
}

GrassBlock::GrassBlock() : Block() {
    textures.push_back(Texture::loadTexture("Textures/Blocks/GrassTop.png"));
    textures.push_back(Texture::loadTexture("Textures/Blocks/GrassSide.png"));
    textures.push_back(Texture::loadTexture("Textures/Blocks/Dirt.png"));
}
