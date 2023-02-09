#include "GrassBlockRenderer.h"
#include "Util/EngineDefaultsClient.h"

size_t GrassBlockRenderer::GetIndexTextureSide(const BlockFaces face) const
{
    if (face == BlockFaces::Top)
    {
        return 0;
    }
    if (face == BlockFaces::Bottom)
    {
        return 2;
    }
    return 1;
}

void GrassBlockRenderer::GenerateTessellationData(const Block* block, TessellationHelper& tessellationHelper, const BlockFaces& face,
                                                  const float& x, const float& y, const float& z, const int& brightness, const float r, const float g, const float b, const float a) const
{
    const BoundingBox blockBounds = block->GetBoundingBox();
    if (face == BlockFaces::Top)
    {
        const float ambientLightFactor = EngineDefaults::ConvertLightLevelToAmbient(brightness);
        const size_t index = GetIndexTextureSide(face);
        const uint16_t triangleIndex1 = tessellationHelper.AddVertex(Vertex(x + blockBounds.GetMinX(), y + blockBounds.GetMaxY(),
                                                                            z + blockBounds.GetMinZ(), 0, 0,
                                                                            r * 94.0F / 255.0F * Y_SIDE_SHADE * ambientLightFactor,
                                                                            g * 157.0F / 255.0F * Y_SIDE_SHADE * ambientLightFactor,
                                                                            b * 52.0F / 255.0F * Y_SIDE_SHADE * ambientLightFactor, a,
                                                                            IndexTextures[index], 0.0F, 1.0F, 0.0F, brightness));
        const uint16_t triangleIndex2 = tessellationHelper.AddVertex(Vertex(x + blockBounds.GetMinX(), y + blockBounds.GetMaxY(),
                                                                            z + blockBounds.GetMaxZ(), 0, 1,
                                                                            r * 94.0F / 255.0F * Y_SIDE_SHADE * ambientLightFactor,
                                                                            g * 157.0F / 255.0F * Y_SIDE_SHADE * ambientLightFactor,
                                                                            b * 52.0F / 255.0F * Y_SIDE_SHADE * ambientLightFactor, a,
                                                                            IndexTextures[index], 0.0F, 1.0F, 0.0F, brightness));
        const uint16_t triangleIndex3 = tessellationHelper.AddVertex(Vertex(x + blockBounds.GetMaxX(), y + blockBounds.GetMaxY(),
                                                                            z + blockBounds.GetMinZ(), 1, 0,
                                                                            94.0F / 255.0F * Y_SIDE_SHADE * ambientLightFactor,
                                                                            157.0F / 255.0F * Y_SIDE_SHADE * ambientLightFactor,
                                                                            52.0F / 255.0F * Y_SIDE_SHADE * ambientLightFactor, a,
                                                                            IndexTextures[index], 0.0F, 1.0F, 0.0F, brightness));
        const uint16_t triangleIndex4 = tessellationHelper.AddVertex(Vertex(x + blockBounds.GetMaxX(), y + blockBounds.GetMaxY(),
                                                                            z + blockBounds.GetMaxZ(), 1, 1,
                                                                            r * 94.0F / 255.0F * Y_SIDE_SHADE * ambientLightFactor,
                                                                            g * 157.0F / 255.0F * Y_SIDE_SHADE * ambientLightFactor,
                                                                            b * 52.0F / 255.0F * Y_SIDE_SHADE * ambientLightFactor, a,
                                                                            IndexTextures[index], 0.0F, 1.0F, 0.0F, brightness));
        tessellationHelper.AddTriangle(triangleIndex1);
        tessellationHelper.AddTriangle(triangleIndex2);
        tessellationHelper.AddTriangle(triangleIndex3);
        tessellationHelper.AddTriangle(triangleIndex4);
        tessellationHelper.AddTriangle(triangleIndex3);
        tessellationHelper.AddTriangle(triangleIndex2);
    }
    else
    {
        BlockRenderer::GenerateTessellationData(block, tessellationHelper, face, x, y, z, brightness, r, g, b, a);
    }
}

GrassBlockRenderer::GrassBlockRenderer()
{
    IndexTextures.push_back(EngineDefaultsClient::RegisterTexture(Texture::LoadTexture("Textures/Blocks/GrassTop.png")));
    IndexTextures.push_back(EngineDefaultsClient::RegisterTexture(Texture::LoadTexture("Textures/Blocks/GrassSide.png")));
    IndexTextures.push_back(EngineDefaultsClient::RegisterTexture(Texture::LoadTexture("Textures/Blocks/Dirt.png")));
}
