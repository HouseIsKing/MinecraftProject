#include "PlayerSelectionHighlight.h"

#include "Util/EngineDefaults.h"

PlayerSelectionHighlight::PlayerSelectionHighlight() : TextureIndex(EngineDefaults::RegisterTexture(Texture::LoadTexture("Textures/HighlightTexture.png"))), BlockHit(nullptr), FaceHit(BlockFaces::Bottom), HitPosition(0, 0, 0)
{
}

void PlayerSelectionHighlight::Reset()
{
	BlockHit = nullptr;
}

void PlayerSelectionHighlight::Render(const int& brightness)
{
	const auto x = static_cast<float>(HitPosition.x);
	const auto y = static_cast<float>(HitPosition.y);
	const auto z = static_cast<float>(HitPosition.z);
	const auto helper = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());
	const float alpha = static_cast<float>(sin(static_cast<double>(helper.count()) / 100.0)) * 0.2F + 0.4F;
	Tessellation.Reset();
	uint16_t triangleIndex1 = 0;
	uint16_t triangleIndex2 = 0;
	uint16_t triangleIndex3 = 0;
	uint16_t triangleIndex4 = 0;
	const BoundingBox blockBounds = BlockHit->GetBoundingBox();
	constexpr float offset = 0.005F;
	switch (FaceHit)
	{
	case BlockFaces::Top:
		triangleIndex1 = Tessellation.AddVertex(Vertex(x + blockBounds.GetMinX(), y + blockBounds.GetMaxY() + offset, z + blockBounds.GetMinZ(), 0, 0, 1.0F, 1.0F, 1.0F, alpha, TextureIndex, 0.0F, 1.0F, 0.0F, brightness));
		triangleIndex2 = Tessellation.AddVertex(Vertex(x + blockBounds.GetMinX(), y + blockBounds.GetMaxY() + offset, z + blockBounds.GetMaxZ(), 0, 1, 1.0F, 1.0F, 1.0F, alpha, TextureIndex, 0.0F, 1.0F, 0.0F, brightness));
		triangleIndex3 = Tessellation.AddVertex(Vertex(x + blockBounds.GetMaxX(), y + blockBounds.GetMaxY() + offset, z + blockBounds.GetMinZ(), 1, 0, 1.0F, 1.0F, 1.0F, alpha, TextureIndex, 0.0F, 1.0F, 0.0F, brightness));
		triangleIndex4 = Tessellation.AddVertex(Vertex(x + blockBounds.GetMaxX(), y + blockBounds.GetMaxY() + offset, z + blockBounds.GetMaxZ(), 1, 1, 1.0F, 1.0F, 1.0F, alpha, TextureIndex, 0.0F, 1.0F, 0.0F, brightness));
		break;
	case BlockFaces::Bottom:
		triangleIndex1 = Tessellation.AddVertex(Vertex(x + blockBounds.GetMinX(), y + blockBounds.GetMinY() - offset, z + blockBounds.GetMinZ(), 0, 0, 1.0F, 1.0F, 1.0F, alpha, TextureIndex, 0.0F, -1.0F, 0.0F, brightness));
		triangleIndex2 = Tessellation.AddVertex(Vertex(x + blockBounds.GetMaxX(), y + blockBounds.GetMinY() - offset, z + blockBounds.GetMinZ(), 1, 0, 1.0F, 1.0F, 1.0F, alpha, TextureIndex, 0.0F, -1.0F, 0.0F, brightness));
		triangleIndex3 = Tessellation.AddVertex(Vertex(x + blockBounds.GetMinX(), y + blockBounds.GetMinY() - offset, z + blockBounds.GetMaxZ(), 0, 1, 1.0F, 1.0F, 1.0F, alpha, TextureIndex, 0.0F, -1.0F, 0.0F, brightness));
		triangleIndex4 = Tessellation.AddVertex(Vertex(x + blockBounds.GetMaxX(), y + blockBounds.GetMinY() - offset, z + blockBounds.GetMaxZ(), 1, 1, 1.0F, 1.0F, 1.0F, alpha, TextureIndex, 0.0F, -1.0F, 0.0F, brightness));
		break;
	case BlockFaces::South:
		triangleIndex1 = Tessellation.AddVertex(Vertex(x + blockBounds.GetMinX(), y + blockBounds.GetMinY(), z + blockBounds.GetMinZ() - offset, 0, 0, 1.0F, 1.0F, 1.0F, alpha, TextureIndex, 0.0F, 0.0F, -1.0F, brightness));
		triangleIndex2 = Tessellation.AddVertex(Vertex(x + blockBounds.GetMinX(), y + blockBounds.GetMaxY(), z + blockBounds.GetMinZ() - offset, 0, 1, 1.0F, 1.0F, 1.0F, alpha, TextureIndex, 0.0F, 0.0F, -1.0F, brightness));
		triangleIndex3 = Tessellation.AddVertex(Vertex(x + blockBounds.GetMaxX(), y + blockBounds.GetMinY(), z + blockBounds.GetMinZ() - offset, 1, 0, 1.0F, 1.0F, 1.0F, alpha, TextureIndex, 0.0F, 0.0F, -1.0F, brightness));
		triangleIndex4 = Tessellation.AddVertex(Vertex(x + blockBounds.GetMaxX(), y + blockBounds.GetMaxY(), z + blockBounds.GetMinZ() - offset, 1, 1, 1.0F, 1.0F, 1.0F, alpha, TextureIndex, 0.0F, 0.0F, -1.0F, brightness));
		break;
	case BlockFaces::North:
		triangleIndex1 = Tessellation.AddVertex(Vertex(x + blockBounds.GetMinX(), y + blockBounds.GetMinY(), z + blockBounds.GetMaxZ() + offset, 0, 0, 1.0F, 1.0F, 1.0F, alpha, TextureIndex, 0.0F, 0.0F, 1.0F, brightness));
		triangleIndex2 = Tessellation.AddVertex(Vertex(x + blockBounds.GetMaxX(), y + blockBounds.GetMinY(), z + blockBounds.GetMaxZ() + offset, 1, 0, 1.0F, 1.0F, 1.0F, alpha, TextureIndex, 0.0F, 0.0F, 1.0F, brightness));
		triangleIndex3 = Tessellation.AddVertex(Vertex(x + blockBounds.GetMinX(), y + blockBounds.GetMaxY(), z + blockBounds.GetMaxZ() + offset, 0, 1, 1.0F, 1.0F, 1.0F, alpha, TextureIndex, 0.0F, 0.0F, 1.0F, brightness));
		triangleIndex4 = Tessellation.AddVertex(Vertex(x + blockBounds.GetMaxX(), y + blockBounds.GetMaxY(), z + blockBounds.GetMaxZ() + offset, 1, 1, 1.0F, 1.0F, 1.0F, alpha, TextureIndex, 0.0F, 0.0F, 1.0F, brightness));
		break;
	case BlockFaces::East:
		triangleIndex1 = Tessellation.AddVertex(Vertex(x + blockBounds.GetMaxX() + offset, y + blockBounds.GetMinY(), z + blockBounds.GetMinZ(), 0, 0, 1.0F, 1.0F, 1.0F, alpha, TextureIndex, 1.0F, 0.0F, 0.0F, brightness));
		triangleIndex2 = Tessellation.AddVertex(Vertex(x + blockBounds.GetMaxX() + offset, y + blockBounds.GetMaxY(), z + blockBounds.GetMinZ(), 0, 1, 1.0F, 1.0F, 1.0F, alpha, TextureIndex, 1.0F, 0.0F, 0.0F, brightness));
		triangleIndex3 = Tessellation.AddVertex(Vertex(x + blockBounds.GetMaxX() + offset, y + blockBounds.GetMinY(), z + blockBounds.GetMaxZ(), 1, 0, 1.0F, 1.0F, 1.0F, alpha, TextureIndex, 1.0F, 0.0F, 0.0F, brightness));
		triangleIndex4 = Tessellation.AddVertex(Vertex(x + blockBounds.GetMaxX() + offset, y + blockBounds.GetMaxY(), z + blockBounds.GetMaxZ(), 1, 1, 1.0F, 1.0F, 1.0F, alpha, TextureIndex, 1.0F, 0.0F, 0.0F, brightness));
		break;
	case BlockFaces::West:
		triangleIndex1 = Tessellation.AddVertex(Vertex(x + blockBounds.GetMinX() - offset, y + blockBounds.GetMinY(), z + blockBounds.GetMinZ(), 0, 0, 1.0F, 1.0F, 1.0F, alpha, TextureIndex, -1.0F, 0.0F, 0.0F, brightness));
		triangleIndex2 = Tessellation.AddVertex(Vertex(x + blockBounds.GetMinX() - offset, y + blockBounds.GetMinY(), z + blockBounds.GetMaxZ(), 1, 0, 1.0F, 1.0F, 1.0F, alpha, TextureIndex, -1.0F, 0.0F, 0.0F, brightness));
		triangleIndex3 = Tessellation.AddVertex(Vertex(x + blockBounds.GetMinX() - offset, y + blockBounds.GetMaxY(), z + blockBounds.GetMinZ(), 0, 1, 1.0F, 1.0F, 1.0F, alpha, TextureIndex, -1.0F, 0.0F, 0.0F, brightness));
		triangleIndex4 = Tessellation.AddVertex(Vertex(x + blockBounds.GetMinX() - offset, y + blockBounds.GetMaxY(), z + blockBounds.GetMaxZ(), 1, 1, 1.0F, 1.0F, 1.0F, alpha, TextureIndex, -1.0F, 0.0F, 0.0F, brightness));
		break;
	}
	Tessellation.AddTriangle(triangleIndex1);
	Tessellation.AddTriangle(triangleIndex2);
	Tessellation.AddTriangle(triangleIndex3);
	Tessellation.AddTriangle(triangleIndex4);
	Tessellation.AddTriangle(triangleIndex3);
	Tessellation.AddTriangle(triangleIndex2);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	glEnable(GL_BLEND);
	Tessellation.Draw();
	glDisable(GL_BLEND);
}