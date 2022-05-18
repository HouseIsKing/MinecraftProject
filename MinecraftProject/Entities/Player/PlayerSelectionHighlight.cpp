#include "PlayerSelectionHighlight.h"

#include "Util/EngineDefaults.h"

PlayerSelectionHighlight::PlayerSelectionHighlight() : Tessellation(EngineDefaults::GetShader()),
                                                       TextureIndex(EngineDefaults::RegisterTexture(
	                                                       Texture::LoadTexture("Textures/HighlightTexture.png")))
{
}

void PlayerSelectionHighlight::Render(const float& x, const float& y, const float& z, const BlockFaces& face, const float& brightness, const Block& blockToDrawUpon)
{
	Tessellation.Reset();
	uint16_t triangleIndex1 = 0;
	uint16_t triangleIndex2 = 0;
	uint16_t triangleIndex3 = 0;
	uint16_t triangleIndex4 = 0;
	const BoundingBox blockBounds = blockToDrawUpon.GetBoundingBox();
	constexpr float offset = 0.0005F;
	switch (face)
	{
	case BlockFaces::Top:
		triangleIndex1 = Tessellation.AddVertex(Vertex(x + blockBounds.GetMinX(), y + blockBounds.GetMaxY() + offset, z + blockBounds.GetMinZ(), 0, 0, 1.0F, 1.0F, 1.0F, 0.8F, TextureIndex, 0.0F, 1.0F, 0.0F, brightness));
		triangleIndex2 = Tessellation.AddVertex(Vertex(x + blockBounds.GetMinX(), y + blockBounds.GetMaxY() + offset, z + blockBounds.GetMaxZ(), 0, 1, 1.0F, 1.0F, 1.0F, 0.8F, TextureIndex, 0.0F, 1.0F, 0.0F, brightness));
		triangleIndex3 = Tessellation.AddVertex(Vertex(x + blockBounds.GetMaxX(), y + blockBounds.GetMaxY() + offset, z + blockBounds.GetMinZ(), 1, 0, 1.0F, 1.0F, 1.0F, 0.8F, TextureIndex, 0.0F, 1.0F, 0.0F, brightness));
		triangleIndex4 = Tessellation.AddVertex(Vertex(x + blockBounds.GetMaxX(), y + blockBounds.GetMaxY() + offset, z + blockBounds.GetMaxZ(), 1, 1, 1.0F, 1.0F, 1.0F, 0.8F, TextureIndex, 0.0F, 1.0F, 0.0F, brightness));
		break;
	case BlockFaces::Bottom:
		triangleIndex1 = Tessellation.AddVertex(Vertex(x + blockBounds.GetMinX(), y + blockBounds.GetMinY() - offset, z + blockBounds.GetMinZ(), 0, 0, 1.0F, 1.0F, 1.0F, 1.0F, TextureIndex, 0.0F, -1.0F, 0.0F, brightness));
		triangleIndex2 = Tessellation.AddVertex(Vertex(x + blockBounds.GetMaxX(), y + blockBounds.GetMinY() - offset, z + blockBounds.GetMinZ(), 1, 0, 1.0F, 1.0F, 1.0F, 1.0F, TextureIndex, 0.0F, -1.0F, 0.0F, brightness));
		triangleIndex3 = Tessellation.AddVertex(Vertex(x + blockBounds.GetMinX(), y + blockBounds.GetMinY() - offset, z + blockBounds.GetMaxZ(), 0, 1, 1.0F, 1.0F, 1.0F, 1.0F, TextureIndex, 0.0F, -1.0F, 0.0F, brightness));
		triangleIndex4 = Tessellation.AddVertex(Vertex(x + blockBounds.GetMaxX(), y + blockBounds.GetMinY() - offset, z + blockBounds.GetMaxZ(), 1, 1, 1.0F, 1.0F, 1.0F, 1.0F, TextureIndex, 0.0F, -1.0F, 0.0F, brightness));
		break;
	case BlockFaces::South:
		triangleIndex1 = Tessellation.AddVertex(Vertex(x + blockBounds.GetMinX(), y + blockBounds.GetMinY(), z + blockBounds.GetMinZ() - offset, 0, 0, 1.0F, 1.0F, 1.0F, 1.0F, TextureIndex, 0.0F, 0.0F, -1.0F, brightness));
		triangleIndex2 = Tessellation.AddVertex(Vertex(x + blockBounds.GetMinX(), y + blockBounds.GetMaxY(), z + blockBounds.GetMinZ() - offset, 0, 1, 1.0F, 1.0F, 1.0F, 1.0F, TextureIndex, 0.0F, 0.0F, -1.0F, brightness));
		triangleIndex3 = Tessellation.AddVertex(Vertex(x + blockBounds.GetMaxX(), y + blockBounds.GetMinY(), z + blockBounds.GetMinZ() - offset, 1, 0, 1.0F, 1.0F, 1.0F, 1.0F, TextureIndex, 0.0F, 0.0F, -1.0F, brightness));
		triangleIndex4 = Tessellation.AddVertex(Vertex(x + blockBounds.GetMaxX(), y + blockBounds.GetMaxY(), z + blockBounds.GetMinZ() - offset, 1, 1, 1.0F, 1.0F, 1.0F, 1.0F, TextureIndex, 0.0F, 0.0F, -1.0F, brightness));
		break;
	case BlockFaces::North:
		triangleIndex1 = Tessellation.AddVertex(Vertex(x + blockBounds.GetMinX(), y + blockBounds.GetMinY(), z + blockBounds.GetMaxZ() + offset, 0, 0, 1.0F, 1.0F, 1.0F, 1.0F, TextureIndex, 0.0F, 0.0F, 1.0F, brightness));
		triangleIndex2 = Tessellation.AddVertex(Vertex(x + blockBounds.GetMaxX(), y + blockBounds.GetMinY(), z + blockBounds.GetMaxZ() + offset, 1, 0, 1.0F, 1.0F, 1.0F, 1.0F, TextureIndex, 0.0F, 0.0F, 1.0F, brightness));
		triangleIndex3 = Tessellation.AddVertex(Vertex(x + blockBounds.GetMinX(), y + blockBounds.GetMaxY(), z + blockBounds.GetMaxZ() + offset, 0, 1, 1.0F, 1.0F, 1.0F, 1.0F, TextureIndex, 0.0F, 0.0F, 1.0F, brightness));
		triangleIndex4 = Tessellation.AddVertex(Vertex(x + blockBounds.GetMaxX(), y + blockBounds.GetMaxY(), z + blockBounds.GetMaxZ() + offset, 1, 1, 1.0F, 1.0F, 1.0F, 1.0F, TextureIndex, 0.0F, 0.0F, 1.0F, brightness));
		break;
	case BlockFaces::East:
		triangleIndex1 = Tessellation.AddVertex(Vertex(x + blockBounds.GetMaxX() + offset, y + blockBounds.GetMinY(), z + blockBounds.GetMinZ(), 0, 0, 1.0F, 1.0F, 1.0F, 1.0F, TextureIndex, 1.0F, 0.0F, 0.0F, brightness));
		triangleIndex2 = Tessellation.AddVertex(Vertex(x + blockBounds.GetMaxX() + offset, y + blockBounds.GetMaxY(), z + blockBounds.GetMinZ(), 0, 1, 1.0F, 1.0F, 1.0F, 1.0F, TextureIndex, 1.0F, 0.0F, 0.0F, brightness));
		triangleIndex3 = Tessellation.AddVertex(Vertex(x + blockBounds.GetMaxX() + offset, y + blockBounds.GetMinY(), z + blockBounds.GetMaxZ(), 1, 0, 1.0F, 1.0F, 1.0F, 1.0F, TextureIndex, 1.0F, 0.0F, 0.0F, brightness));
		triangleIndex4 = Tessellation.AddVertex(Vertex(x + blockBounds.GetMaxX() + offset, y + blockBounds.GetMaxY(), z + blockBounds.GetMaxZ(), 1, 1, 1.0F, 1.0F, 1.0F, 1.0F, TextureIndex, 1.0F, 0.0F, 0.0F, brightness));
		break;
	case BlockFaces::West:
		triangleIndex1 = Tessellation.AddVertex(Vertex(x + blockBounds.GetMinX() - offset, y + blockBounds.GetMinY(), z + blockBounds.GetMinZ(), 0, 0, 1.0F, 1.0F, 1.0F, 1.0F, TextureIndex, -1.0F, 0.0F, 0.0F, brightness));
		triangleIndex2 = Tessellation.AddVertex(Vertex(x + blockBounds.GetMinX() - offset, y + blockBounds.GetMinY(), z + blockBounds.GetMaxZ(), 1, 0, 1.0F, 1.0F, 1.0F, 1.0F, TextureIndex, -1.0F, 0.0F, 0.0F, brightness));
		triangleIndex3 = Tessellation.AddVertex(Vertex(x + blockBounds.GetMinX() - offset, y + blockBounds.GetMaxY(), z + blockBounds.GetMinZ(), 0, 1, 1.0F, 1.0F, 1.0F, 1.0F, TextureIndex, -1.0F, 0.0F, 0.0F, brightness));
		triangleIndex4 = Tessellation.AddVertex(Vertex(x + blockBounds.GetMinX() - offset, y + blockBounds.GetMaxY(), z + blockBounds.GetMaxZ(), 1, 1, 1.0F, 1.0F, 1.0F, 1.0F, TextureIndex, -1.0F, 0.0F, 0.0F, brightness));
		break;
	}
	Tessellation.AddTriangle(triangleIndex1);
	Tessellation.AddTriangle(triangleIndex2);
	Tessellation.AddTriangle(triangleIndex3);
	Tessellation.AddTriangle(triangleIndex4);
	Tessellation.AddTriangle(triangleIndex3);
	Tessellation.AddTriangle(triangleIndex2);
	Tessellation.Draw();
}
