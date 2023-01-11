#include "Block.h"

#include "Entities/ParticleEntity.h"
#include "Util/EngineDefaults.h"

Block::Block() : Block(0, 0, 0, 1, 1, 1)
{
}

Block::Block(const float minX, const float minY, const float minZ, const float maxX, const float maxY, const float maxZ) : BlockBounds(minX, minY, minZ, maxX, maxY, maxZ)
{
}

Block::~Block() = default;

BoundingBox Block::GetBoundingBox() const
{
	return BlockBounds;
}

void Block::GenerateTessellationData(TessellationHelper& tessellationHelper, const BlockFaces& face, const float& x,
                                     const float& y, const float& z, const int& brightness) const
{
	GenerateTessellationData(tessellationHelper, face, x, y, z, brightness, 1.0F, 1.0F, 1.0F, 1.0F);
}

void Block::GenerateTessellationDataForAllFaces(TessellationHelper& tessellationHelper, const float& x, const float& y, const float& z, const int& brightness, float r, float g, float b, float a) const
{
	GenerateTessellationData(tessellationHelper, BlockFaces::Top, x, y, z, brightness, r, g, b, a);
	GenerateTessellationData(tessellationHelper, BlockFaces::Bottom, x, y, z, brightness, r, g, b, a);
	GenerateTessellationData(tessellationHelper, BlockFaces::North, x, y, z, brightness, r, g, b, a);
	GenerateTessellationData(tessellationHelper, BlockFaces::South, x, y, z, brightness, r, g, b, a);
	GenerateTessellationData(tessellationHelper, BlockFaces::West, x, y, z, brightness, r, g, b, a);
	GenerateTessellationData(tessellationHelper, BlockFaces::East, x, y, z, brightness, r, g, b, a);
}

void Block::GenerateTessellationData(TessellationHelper& tessellationHelper, const BlockFaces& face, const float& x,
                                     const float& y, const float& z, const int& brightness, const float r, const float g, const float b, const float a) const
{
	uint16_t triangleIndex1 = 0;
	uint16_t triangleIndex2 = 0;
	uint16_t triangleIndex3 = 0;
	uint16_t triangleIndex4 = 0;
	const float ambientLightFactor = EngineDefaults::ConvertLightLevelToAmbient(brightness);
	const size_t index = GetIndexTextureSide(face);
	switch (face)
	{
	case BlockFaces::Top:
		triangleIndex1 = tessellationHelper.AddVertex(Vertex(x + BlockBounds.GetMinX(), y + BlockBounds.GetMaxY(), z + BlockBounds.GetMinZ(), 0, 0, r * Y_SIDE_SHADE * ambientLightFactor, g * Y_SIDE_SHADE * ambientLightFactor, b * Y_SIDE_SHADE * ambientLightFactor, a, IndexTextures[index], 0.0F, 1.0F, 0.0F, brightness));
		triangleIndex2 = tessellationHelper.AddVertex(Vertex(x + BlockBounds.GetMinX(), y + BlockBounds.GetMaxY(), z + BlockBounds.GetMaxZ(), 0, 1, r * Y_SIDE_SHADE * ambientLightFactor, g * Y_SIDE_SHADE * ambientLightFactor, b * Y_SIDE_SHADE * ambientLightFactor, a, IndexTextures[index], 0.0F, 1.0F, 0.0F, brightness));
		triangleIndex3 = tessellationHelper.AddVertex(Vertex(x + BlockBounds.GetMaxX(), y + BlockBounds.GetMaxY(), z + BlockBounds.GetMinZ(), 1, 0, r * Y_SIDE_SHADE * ambientLightFactor, g * Y_SIDE_SHADE * ambientLightFactor, b * Y_SIDE_SHADE * ambientLightFactor, a, IndexTextures[index], 0.0F, 1.0F, 0.0F, brightness));
		triangleIndex4 = tessellationHelper.AddVertex(Vertex(x + BlockBounds.GetMaxX(), y + BlockBounds.GetMaxY(), z + BlockBounds.GetMaxZ(), 1, 1, r * Y_SIDE_SHADE * ambientLightFactor, g * Y_SIDE_SHADE * ambientLightFactor, b * Y_SIDE_SHADE * ambientLightFactor, a, IndexTextures[index], 0.0F, 1.0F, 0.0F, brightness));
		break;
	case BlockFaces::Bottom:
		triangleIndex1 = tessellationHelper.AddVertex(Vertex(x + BlockBounds.GetMinX(), y + BlockBounds.GetMinY(), z + BlockBounds.GetMinZ(), 0, 0, r * Y_SIDE_SHADE * ambientLightFactor, g * Y_SIDE_SHADE * ambientLightFactor, b * Y_SIDE_SHADE * ambientLightFactor, a, IndexTextures[index], 0.0F, -1.0F, 0.0F, brightness));
		triangleIndex2 = tessellationHelper.AddVertex(Vertex(x + BlockBounds.GetMaxX(), y + BlockBounds.GetMinY(), z + BlockBounds.GetMinZ(), 1, 0, r * Y_SIDE_SHADE * ambientLightFactor, g * Y_SIDE_SHADE * ambientLightFactor, b * Y_SIDE_SHADE * ambientLightFactor, a, IndexTextures[index], 0.0F, -1.0F, 0.0F, brightness));
		triangleIndex3 = tessellationHelper.AddVertex(Vertex(x + BlockBounds.GetMinX(), y + BlockBounds.GetMinY(), z + BlockBounds.GetMaxZ(), 0, 1, r * Y_SIDE_SHADE * ambientLightFactor, g * Y_SIDE_SHADE * ambientLightFactor, b * Y_SIDE_SHADE * ambientLightFactor, a, IndexTextures[index], 0.0F, -1.0F, 0.0F, brightness));
		triangleIndex4 = tessellationHelper.AddVertex(Vertex(x + BlockBounds.GetMaxX(), y + BlockBounds.GetMinY(), z + BlockBounds.GetMaxZ(), 1, 1, r * Y_SIDE_SHADE * ambientLightFactor, g * Y_SIDE_SHADE * ambientLightFactor, b * Y_SIDE_SHADE * ambientLightFactor, a, IndexTextures[index], 0.0F, -1.0F, 0.0F, brightness));
		break;
	case BlockFaces::South:
		triangleIndex1 = tessellationHelper.AddVertex(Vertex(x + BlockBounds.GetMinX(), y + BlockBounds.GetMinY(), z + BlockBounds.GetMinZ(), 0, 0, r * Z_SIDE_SHADE * ambientLightFactor, g * Z_SIDE_SHADE * ambientLightFactor, b * Z_SIDE_SHADE * ambientLightFactor, a, IndexTextures[index], 0.0F, 0.0F, -1.0F, brightness));
		triangleIndex2 = tessellationHelper.AddVertex(Vertex(x + BlockBounds.GetMinX(), y + BlockBounds.GetMaxY(), z + BlockBounds.GetMinZ(), 0, 1, r * Z_SIDE_SHADE * ambientLightFactor, g * Z_SIDE_SHADE * ambientLightFactor, b * Z_SIDE_SHADE * ambientLightFactor, a, IndexTextures[index], 0.0F, 0.0F, -1.0F, brightness));
		triangleIndex3 = tessellationHelper.AddVertex(Vertex(x + BlockBounds.GetMaxX(), y + BlockBounds.GetMinY(), z + BlockBounds.GetMinZ(), 1, 0, r * Z_SIDE_SHADE * ambientLightFactor, g * Z_SIDE_SHADE * ambientLightFactor, b * Z_SIDE_SHADE * ambientLightFactor, a, IndexTextures[index], 0.0F, 0.0F, -1.0F, brightness));
		triangleIndex4 = tessellationHelper.AddVertex(Vertex(x + BlockBounds.GetMaxX(), y + BlockBounds.GetMaxY(), z + BlockBounds.GetMinZ(), 1, 1, r * Z_SIDE_SHADE * ambientLightFactor, g * Z_SIDE_SHADE * ambientLightFactor, b * Z_SIDE_SHADE * ambientLightFactor, a, IndexTextures[index], 0.0F, 0.0F, -1.0F, brightness));
		break;
	case BlockFaces::North:
		triangleIndex1 = tessellationHelper.AddVertex(Vertex(x + BlockBounds.GetMinX(), y + BlockBounds.GetMinY(), z + BlockBounds.GetMaxZ(), 0, 0, r * Z_SIDE_SHADE * ambientLightFactor, g * Z_SIDE_SHADE * ambientLightFactor, b * Z_SIDE_SHADE * ambientLightFactor, a, IndexTextures[index], 0.0F, 0.0F, 1.0F, brightness));
		triangleIndex2 = tessellationHelper.AddVertex(Vertex(x + BlockBounds.GetMaxX(), y + BlockBounds.GetMinY(), z + BlockBounds.GetMaxZ(), 1, 0, r * Z_SIDE_SHADE * ambientLightFactor, g * Z_SIDE_SHADE * ambientLightFactor, b * Z_SIDE_SHADE * ambientLightFactor, a, IndexTextures[index], 0.0F, 0.0F, 1.0F, brightness));
		triangleIndex3 = tessellationHelper.AddVertex(Vertex(x + BlockBounds.GetMinX(), y + BlockBounds.GetMaxY(), z + BlockBounds.GetMaxZ(), 0, 1, r * Z_SIDE_SHADE * ambientLightFactor, g * Z_SIDE_SHADE * ambientLightFactor, b * Z_SIDE_SHADE * ambientLightFactor, a, IndexTextures[index], 0.0F, 0.0F, 1.0F, brightness));
		triangleIndex4 = tessellationHelper.AddVertex(Vertex(x + BlockBounds.GetMaxX(), y + BlockBounds.GetMaxY(), z + BlockBounds.GetMaxZ(), 1, 1, r * Z_SIDE_SHADE * ambientLightFactor, g * Z_SIDE_SHADE * ambientLightFactor, b * Z_SIDE_SHADE * ambientLightFactor, a, IndexTextures[index], 0.0F, 0.0F, 1.0F, brightness));
		break;
	case BlockFaces::East:
		triangleIndex1 = tessellationHelper.AddVertex(Vertex(x + BlockBounds.GetMaxX(), y + BlockBounds.GetMinY(), z + BlockBounds.GetMinZ(), 0, 0, r * X_SIDE_SHADE * ambientLightFactor, g * X_SIDE_SHADE * ambientLightFactor, b * X_SIDE_SHADE * ambientLightFactor, a, IndexTextures[index], 1.0F, 0.0F, 0.0F, brightness));
		triangleIndex2 = tessellationHelper.AddVertex(Vertex(x + BlockBounds.GetMaxX(), y + BlockBounds.GetMaxY(), z + BlockBounds.GetMinZ(), 0, 1, r * X_SIDE_SHADE * ambientLightFactor, g * X_SIDE_SHADE * ambientLightFactor, b * X_SIDE_SHADE * ambientLightFactor, a, IndexTextures[index], 1.0F, 0.0F, 0.0F, brightness));
		triangleIndex3 = tessellationHelper.AddVertex(Vertex(x + BlockBounds.GetMaxX(), y + BlockBounds.GetMinY(), z + BlockBounds.GetMaxZ(), 1, 0, r * X_SIDE_SHADE * ambientLightFactor, g * X_SIDE_SHADE * ambientLightFactor, b * X_SIDE_SHADE * ambientLightFactor, a, IndexTextures[index], 1.0F, 0.0F, 0.0F, brightness));
		triangleIndex4 = tessellationHelper.AddVertex(Vertex(x + BlockBounds.GetMaxX(), y + BlockBounds.GetMaxY(), z + BlockBounds.GetMaxZ(), 1, 1, r * X_SIDE_SHADE * ambientLightFactor, g * X_SIDE_SHADE * ambientLightFactor, b * X_SIDE_SHADE * ambientLightFactor, a, IndexTextures[index], 1.0F, 0.0F, 0.0F, brightness));
		break;
	case BlockFaces::West:
		triangleIndex1 = tessellationHelper.AddVertex(Vertex(x + BlockBounds.GetMinX(), y + BlockBounds.GetMinY(), z + BlockBounds.GetMinZ(), 0, 0, r * X_SIDE_SHADE * ambientLightFactor, g * X_SIDE_SHADE * ambientLightFactor, b * X_SIDE_SHADE * ambientLightFactor, a, IndexTextures[index], -1.0F, 0.0F, 0.0F, brightness));
		triangleIndex2 = tessellationHelper.AddVertex(Vertex(x + BlockBounds.GetMinX(), y + BlockBounds.GetMinY(), z + BlockBounds.GetMaxZ(), 1, 0, r * X_SIDE_SHADE * ambientLightFactor, g * X_SIDE_SHADE * ambientLightFactor, b * X_SIDE_SHADE * ambientLightFactor, a, IndexTextures[index], -1.0F, 0.0F, 0.0F, brightness));
		triangleIndex3 = tessellationHelper.AddVertex(Vertex(x + BlockBounds.GetMinX(), y + BlockBounds.GetMaxY(), z + BlockBounds.GetMinZ(), 0, 1, r * X_SIDE_SHADE * ambientLightFactor, g * X_SIDE_SHADE * ambientLightFactor, b * X_SIDE_SHADE * ambientLightFactor, a, IndexTextures[index], -1.0F, 0.0F, 0.0F, brightness));
		triangleIndex4 = tessellationHelper.AddVertex(Vertex(x + BlockBounds.GetMinX(), y + BlockBounds.GetMaxY(), z + BlockBounds.GetMaxZ(), 1, 1, r * X_SIDE_SHADE * ambientLightFactor, g * X_SIDE_SHADE * ambientLightFactor, b * X_SIDE_SHADE * ambientLightFactor, a, IndexTextures[index], -1.0F, 0.0F, 0.0F, brightness));
		break;
	}
	tessellationHelper.AddTriangle(triangleIndex1);
	tessellationHelper.AddTriangle(triangleIndex2);
	tessellationHelper.AddTriangle(triangleIndex3);
	tessellationHelper.AddTriangle(triangleIndex4);
	tessellationHelper.AddTriangle(triangleIndex3);
	tessellationHelper.AddTriangle(triangleIndex2);
}

void Block::GenerateTessellationData(GuiTessellation& tessellationHelper, const BlockFaces& face,
                                     const mat4x4 transformationMatrix) const
{
	uint16_t triangleIndex1 = 0;
	uint16_t triangleIndex2 = 0;
	uint16_t triangleIndex3 = 0;
	uint16_t triangleIndex4 = 0;
	const size_t index = GetIndexTextureSide(face);
	switch (face)
	{
	case BlockFaces::Top:
		triangleIndex1 = tessellationHelper.AddVertex(GuiVertex(vec3(transformationMatrix * vec4(BlockBounds.GetMinX() - 2, BlockBounds.GetMaxY(), BlockBounds.GetMinZ(), 1.0F)), 0, 0, Y_SIDE_SHADE, Y_SIDE_SHADE, Y_SIDE_SHADE, 1.0F, IndexTextures[index]));
		triangleIndex2 = tessellationHelper.AddVertex(GuiVertex(vec3(transformationMatrix * vec4(BlockBounds.GetMinX() - 2, BlockBounds.GetMaxY(), BlockBounds.GetMaxZ(), 1.0F)), 0, 1, Y_SIDE_SHADE, Y_SIDE_SHADE, Y_SIDE_SHADE, 1.0F, IndexTextures[index]));
		triangleIndex3 = tessellationHelper.AddVertex(GuiVertex(vec3(transformationMatrix * vec4(BlockBounds.GetMaxX() - 2, BlockBounds.GetMaxY(), BlockBounds.GetMinZ(), 1.0F)), 1, 0, Y_SIDE_SHADE, Y_SIDE_SHADE, Y_SIDE_SHADE, 1.0F, IndexTextures[index]));
		triangleIndex4 = tessellationHelper.AddVertex(GuiVertex(vec3(transformationMatrix * vec4(BlockBounds.GetMaxX() - 2, BlockBounds.GetMaxY(), BlockBounds.GetMaxZ(), 1.0F)), 1, 1, Y_SIDE_SHADE, Y_SIDE_SHADE, Y_SIDE_SHADE, 1.0F, IndexTextures[index]));
		break;
	case BlockFaces::Bottom:
		triangleIndex1 = tessellationHelper.AddVertex(GuiVertex(vec3(transformationMatrix * vec4(BlockBounds.GetMinX() - 2, BlockBounds.GetMinY(), BlockBounds.GetMinZ(), 1.0F)), 0, 0, Y_SIDE_SHADE, Y_SIDE_SHADE, Y_SIDE_SHADE, 1.0F, IndexTextures[index]));
		triangleIndex2 = tessellationHelper.AddVertex(GuiVertex(vec3(transformationMatrix * vec4(BlockBounds.GetMaxX() - 2, BlockBounds.GetMinY(), BlockBounds.GetMinZ(), 1.0F)), 1, 0, Y_SIDE_SHADE, Y_SIDE_SHADE, Y_SIDE_SHADE, 1.0F, IndexTextures[index]));
		triangleIndex3 = tessellationHelper.AddVertex(GuiVertex(vec3(transformationMatrix * vec4(BlockBounds.GetMinX() - 2, BlockBounds.GetMinY(), BlockBounds.GetMaxZ(), 1.0F)), 0, 1, Y_SIDE_SHADE, Y_SIDE_SHADE, Y_SIDE_SHADE, 1.0F, IndexTextures[index]));
		triangleIndex4 = tessellationHelper.AddVertex(GuiVertex(vec3(transformationMatrix * vec4(BlockBounds.GetMaxX() - 2, BlockBounds.GetMinY(), BlockBounds.GetMaxZ(), 1.0F)), 1, 1, Y_SIDE_SHADE, Y_SIDE_SHADE, Y_SIDE_SHADE, 1.0F, IndexTextures[index]));
		break;
	case BlockFaces::South:
		triangleIndex1 = tessellationHelper.AddVertex(GuiVertex(vec3(transformationMatrix * vec4(BlockBounds.GetMinX() - 2, BlockBounds.GetMinY(), BlockBounds.GetMinZ(), 1.0F)), 0, 0, Z_SIDE_SHADE, Z_SIDE_SHADE, Z_SIDE_SHADE, 1.0F, IndexTextures[index]));
		triangleIndex2 = tessellationHelper.AddVertex(GuiVertex(vec3(transformationMatrix * vec4(BlockBounds.GetMinX() - 2, BlockBounds.GetMaxY(), BlockBounds.GetMinZ(), 1.0F)), 0, 1, Z_SIDE_SHADE, Z_SIDE_SHADE, Z_SIDE_SHADE, 1.0F, IndexTextures[index]));
		triangleIndex3 = tessellationHelper.AddVertex(GuiVertex(vec3(transformationMatrix * vec4(BlockBounds.GetMaxX() - 2, BlockBounds.GetMinY(), BlockBounds.GetMinZ(), 1.0F)), 1, 0, Z_SIDE_SHADE, Z_SIDE_SHADE, Z_SIDE_SHADE, 1.0F, IndexTextures[index]));
		triangleIndex4 = tessellationHelper.AddVertex(GuiVertex(vec3(transformationMatrix * vec4(BlockBounds.GetMaxX() - 2, BlockBounds.GetMaxY(), BlockBounds.GetMinZ(), 1.0F)), 1, 1, Z_SIDE_SHADE, Z_SIDE_SHADE, Z_SIDE_SHADE, 1.0F, IndexTextures[index]));
		break;
	case BlockFaces::North:
		triangleIndex1 = tessellationHelper.AddVertex(GuiVertex(vec3(transformationMatrix * vec4(BlockBounds.GetMinX() - 2, BlockBounds.GetMinY(), BlockBounds.GetMaxZ(), 1.0F)), 0, 0, Z_SIDE_SHADE, Z_SIDE_SHADE, Z_SIDE_SHADE, 1.0F, IndexTextures[index]));
		triangleIndex2 = tessellationHelper.AddVertex(GuiVertex(vec3(transformationMatrix * vec4(BlockBounds.GetMaxX() - 2, BlockBounds.GetMinY(), BlockBounds.GetMaxZ(), 1.0F)), 1, 0, Z_SIDE_SHADE, Z_SIDE_SHADE, Z_SIDE_SHADE, 1.0F, IndexTextures[index]));
		triangleIndex3 = tessellationHelper.AddVertex(GuiVertex(vec3(transformationMatrix * vec4(BlockBounds.GetMinX() - 2, BlockBounds.GetMaxY(), BlockBounds.GetMaxZ(), 1.0F)), 0, 1, Z_SIDE_SHADE, Z_SIDE_SHADE, Z_SIDE_SHADE, 1.0F, IndexTextures[index]));
		triangleIndex4 = tessellationHelper.AddVertex(GuiVertex(vec3(transformationMatrix * vec4(BlockBounds.GetMaxX() - 2, BlockBounds.GetMaxY(), BlockBounds.GetMaxZ(), 1.0F)), 1, 1, Z_SIDE_SHADE, Z_SIDE_SHADE, Z_SIDE_SHADE, 1.0F, IndexTextures[index]));
		break;
	case BlockFaces::East:
		triangleIndex1 = tessellationHelper.AddVertex(GuiVertex(vec3(transformationMatrix * vec4(BlockBounds.GetMaxX() - 2, BlockBounds.GetMinY(), BlockBounds.GetMinZ(), 1.0F)), 0, 0, X_SIDE_SHADE, X_SIDE_SHADE, X_SIDE_SHADE, 1.0F, IndexTextures[index]));
		triangleIndex2 = tessellationHelper.AddVertex(GuiVertex(vec3(transformationMatrix * vec4(BlockBounds.GetMaxX() - 2, BlockBounds.GetMaxY(), BlockBounds.GetMinZ(), 1.0F)), 0, 1, X_SIDE_SHADE, X_SIDE_SHADE, X_SIDE_SHADE, 1.0F, IndexTextures[index]));
		triangleIndex3 = tessellationHelper.AddVertex(GuiVertex(vec3(transformationMatrix * vec4(BlockBounds.GetMaxX() - 2, BlockBounds.GetMinY(), BlockBounds.GetMaxZ(), 1.0F)), 1, 0, X_SIDE_SHADE, X_SIDE_SHADE, X_SIDE_SHADE, 1.0F, IndexTextures[index]));
		triangleIndex4 = tessellationHelper.AddVertex(GuiVertex(vec3(transformationMatrix * vec4(BlockBounds.GetMaxX() - 2, BlockBounds.GetMaxY(), BlockBounds.GetMaxZ(), 1.0F)), 1, 1, X_SIDE_SHADE, X_SIDE_SHADE, X_SIDE_SHADE, 1.0F, IndexTextures[index]));
		break;
	case BlockFaces::West:
		triangleIndex1 = tessellationHelper.AddVertex(GuiVertex(vec3(transformationMatrix * vec4(BlockBounds.GetMinX() - 2, BlockBounds.GetMinY(), BlockBounds.GetMinZ(), 1.0F)), 0, 0, X_SIDE_SHADE, X_SIDE_SHADE, X_SIDE_SHADE, 1.0F, IndexTextures[index]));
		triangleIndex2 = tessellationHelper.AddVertex(GuiVertex(vec3(transformationMatrix * vec4(BlockBounds.GetMinX() - 2, BlockBounds.GetMinY(), BlockBounds.GetMaxZ(), 1.0F)), 1, 0, X_SIDE_SHADE, X_SIDE_SHADE, X_SIDE_SHADE, 1.0F, IndexTextures[index]));
		triangleIndex3 = tessellationHelper.AddVertex(GuiVertex(vec3(transformationMatrix * vec4(BlockBounds.GetMinX() - 2, BlockBounds.GetMaxY(), BlockBounds.GetMinZ(), 1.0F)), 0, 1, X_SIDE_SHADE, X_SIDE_SHADE, X_SIDE_SHADE, 1.0F, IndexTextures[index]));
		triangleIndex4 = tessellationHelper.AddVertex(GuiVertex(vec3(transformationMatrix * vec4(BlockBounds.GetMinX() - 2, BlockBounds.GetMaxY(), BlockBounds.GetMaxZ(), 1.0F)), 1, 1, X_SIDE_SHADE, X_SIDE_SHADE, X_SIDE_SHADE, 1.0F, IndexTextures[index]));
		break;
	}
	tessellationHelper.AddTriangle(triangleIndex1);
	tessellationHelper.AddTriangle(triangleIndex2);
	tessellationHelper.AddTriangle(triangleIndex3);
	tessellationHelper.AddTriangle(triangleIndex4);
	tessellationHelper.AddTriangle(triangleIndex3);
	tessellationHelper.AddTriangle(triangleIndex2);
}

void Block::Tick(SinglePlayerWorld* /*world*/, int /*x*/, int /*y*/, int /*z*/) const
{
}

void Block::OnBreak(SinglePlayerWorld* /*world*/, const int x, const int y, const int z) const
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			for (int k = 0; k < 4; k++)
			{
				const float posX = static_cast<float>(x) + (static_cast<float>(i) + 0.5F) / 4.0F;
				const float posY = static_cast<float>(y) + (static_cast<float>(j) + 0.5F) / 4.0F;
				const float posZ = static_cast<float>(z) + (static_cast<float>(k) + 0.5F) / 4.0F;
				new ParticleEntity(posX, posY, posZ, posX - static_cast<float>(x) - 0.5F, posY - static_cast<float>(y) - 0.5F, posZ - static_cast<float>(z) - 0.5F, this);
			}
		}
	}
}

uint16_t Block::GetTextureFromIndex(const size_t& index) const
{
	return IndexTextures[index];
}

DrawType Block::GetDrawType() const
{
	return DrawType::Default;
}

bool Block::IsSolidBlock() const
{
	return true;
}

bool Block::IsBlockingLight() const
{
	return true;
}
