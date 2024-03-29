#include "SaplingBlockRenderer.h"
#include "Util/EngineDefaultsClient.h"
#include <glm/ext/scalar_constants.hpp>

SaplingBlockRenderer::SaplingBlockRenderer()
{
    IndexTextures.push_back(EngineDefaultsClient::RegisterTexture(Texture::LoadTexture("Textures/Blocks/Sapling.png")));
}

size_t SaplingBlockRenderer::GetIndexTextureSide(BlockFaces /*face*/) const
{
    return 0;
}

void SaplingBlockRenderer::GenerateTessellationDataForAllFaces(const Block* block, TessellationHelper& tessellationHelper, const float& x,
                                                               const float& y, const float& z, const int& brightness, const float r, const float g, const float b, const float a) const
{
    GenerateTessellationData(block, tessellationHelper, BlockFaces::Top, x, y, z, brightness, r, g, b, a);
}

void SaplingBlockRenderer::GenerateTessellationData(const Block* /*block*/, TessellationHelper& tessellationHelper, const BlockFaces& face,
                                                    const float& x, const float& y, const float& z, const int& brightness, const float r, const float g, const float b, const float a) const
{
    const float ambientLightFactor = EngineDefaults::ConvertLightLevelToAmbient(brightness);
    const uint16_t textureIndex = IndexTextures[GetIndexTextureSide(face)];
    for (int i = 0; i < 2; i++)
    {
        constexpr auto pi = glm::pi<float>();
        const float xa = sin(static_cast<float>(i) * pi / 2.0F + 0.25F * pi) * 0.5F;
        const float za = cos(static_cast<float>(i) * pi / 2.0F + 0.25F * pi) * 0.5F;
        const float x0 = x + 0.5F - xa;
        const float x1 = x + 0.5F + xa;
        const float z0 = z + 0.5F - za;
        const float z1 = z + 0.5F + za;
        const float y0 = y;
        const float y1 = y + 1.0F;
        const uint16_t triangleIndex1 = tessellationHelper.AddVertex(Vertex(x0, y1, z0, 0.0F, 1.0F, r * ambientLightFactor,
                                                                            g * ambientLightFactor, b * ambientLightFactor, a,
                                                                            textureIndex, 0.0F, 0.0F, 0.0F, brightness, 1));
        const uint16_t triangleIndex2 = tessellationHelper.AddVertex(Vertex(x1, y1, z1, 1.0F, 1.0F, r * ambientLightFactor,
                                                                            g * ambientLightFactor, b * ambientLightFactor, a,
                                                                            textureIndex, 0.0F, 0.0F, 0.0F, brightness, 1));
        const uint16_t triangleIndex3 = tessellationHelper.AddVertex(Vertex(x1, y0, z1, 1.0F, 0.0F, r * ambientLightFactor,
                                                                            g * ambientLightFactor, b * ambientLightFactor, a,
                                                                            textureIndex, 0.0F, 0.0F, 0.0F, brightness, 0));
        const uint16_t triangleIndex4 = tessellationHelper.AddVertex(Vertex(x0, y0, z0, 0.0F, 0.0F, r * ambientLightFactor,
                                                                            g * ambientLightFactor, b * ambientLightFactor, a,
                                                                            textureIndex, 0.0F, 0.0F, 0.0F, brightness, 0));
        tessellationHelper.AddTriangle(triangleIndex1);
        tessellationHelper.AddTriangle(triangleIndex2);
        tessellationHelper.AddTriangle(triangleIndex3);
        tessellationHelper.AddTriangle(triangleIndex1);
        tessellationHelper.AddTriangle(triangleIndex3);
        tessellationHelper.AddTriangle(triangleIndex4);
        tessellationHelper.AddTriangle(triangleIndex1);
        tessellationHelper.AddTriangle(triangleIndex3);
        tessellationHelper.AddTriangle(triangleIndex2);
        tessellationHelper.AddTriangle(triangleIndex1);
        tessellationHelper.AddTriangle(triangleIndex4);
        tessellationHelper.AddTriangle(triangleIndex3);
    }
}

void SaplingBlockRenderer::GenerateTessellationData(const Block* /*block*/, GuiTessellation& tessellationHelper, const BlockFaces& face,
                                                    const glm::mat4x4 transformationMatrix) const
{
    const uint16_t index = IndexTextures[GetIndexTextureSide(face)];
    for (int i = 0; i < 2; i++)
    {
        constexpr auto pi = glm::pi<float>();
        const float xa = sin(static_cast<float>(i) * pi / 2.0F + 0.25F * pi) * 0.5F;
        const float za = cos(static_cast<float>(i) * pi / 2.0F + 0.25F * pi) * 0.5F;
        const float x0 = -1.5F - xa;
        const float x1 = -1.5F + xa;
        const float z0 = 0.5F - za;
        const float z1 = 0.5F + za;
        constexpr float y0 = 0.0F;
        constexpr float y1 = 1.0F;
        const uint16_t triangleIndex1 = tessellationHelper.AddVertex(GuiVertex(glm::vec3(transformationMatrix * glm::vec4(x0, y1, z0, 1.0F)), 0.0F, 1.0F, 1.0F,
                                                                               1.0F, 1.0F, 1.0F, index, 1));
        const uint16_t triangleIndex2 = tessellationHelper.AddVertex(GuiVertex(glm::vec3(transformationMatrix * glm::vec4(x1, y1, z1, 1.0F)), 1.0F, 1.0F, 1.0F,
                                                                               1.0F, 1.0F, 1.0F, index, 1));
        const uint16_t triangleIndex3 = tessellationHelper.AddVertex(GuiVertex(glm::vec3(transformationMatrix * glm::vec4(x1, y0, z1, 1.0F)), 1.0F, 0.0F, 1.0F,
                                                                               1.0F, 1.0F, 1.0F, index, 0));
        const uint16_t triangleIndex4 = tessellationHelper.AddVertex(GuiVertex(glm::vec3(transformationMatrix * glm::vec4(x0, y0, z0, 1.0F)), 0.0F, 0.0F, 1.0F,
                                                                               1.0F, 1.0F, 1.0F, index, 0));
        tessellationHelper.AddTriangle(triangleIndex1);
        tessellationHelper.AddTriangle(triangleIndex2);
        tessellationHelper.AddTriangle(triangleIndex3);
        tessellationHelper.AddTriangle(triangleIndex1);
        tessellationHelper.AddTriangle(triangleIndex3);
        tessellationHelper.AddTriangle(triangleIndex4);
        tessellationHelper.AddTriangle(triangleIndex1);
        tessellationHelper.AddTriangle(triangleIndex3);
        tessellationHelper.AddTriangle(triangleIndex2);
        tessellationHelper.AddTriangle(triangleIndex1);
        tessellationHelper.AddTriangle(triangleIndex4);
        tessellationHelper.AddTriangle(triangleIndex3);
    }
}

DrawType SaplingBlockRenderer::GetDrawType() const
{
    return DrawType::Sapling;
}
