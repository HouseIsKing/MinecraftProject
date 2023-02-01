#include "GrassBlock.h"

#include "Util/EngineDefaults.h"
#include "World/SP/SinglePlayerWorld.h"

size_t GrassBlock::GetIndexTextureSide(const BlockFaces face) const
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

void GrassBlock::GenerateTessellationData(TessellationHelper& tessellationHelper, const BlockFaces& face,
                                          const float& x, const float& y, const float& z, const int& brightness, const float r, const float g, const float b, const float a) const
{
    if (face == BlockFaces::Top)
    {
        const float ambientLightFactor = EngineDefaults::ConvertLightLevelToAmbient(brightness);
        const size_t index = GetIndexTextureSide(face);
        const uint16_t triangleIndex1 = tessellationHelper.AddVertex(Vertex(x + BlockBounds.GetMinX(), y + BlockBounds.GetMaxY(),
                                                                            z + BlockBounds.GetMinZ(), 0, 0,
                                                                            r * 94.0F / 255.0F * Y_SIDE_SHADE * ambientLightFactor,
                                                                            g * 157.0F / 255.0F * Y_SIDE_SHADE * ambientLightFactor,
                                                                            b * 52.0F / 255.0F * Y_SIDE_SHADE * ambientLightFactor, a,
                                                                            IndexTextures[index], 0.0F, 1.0F, 0.0F, brightness));
        const uint16_t triangleIndex2 = tessellationHelper.AddVertex(Vertex(x + BlockBounds.GetMinX(), y + BlockBounds.GetMaxY(),
                                                                            z + BlockBounds.GetMaxZ(), 0, 1,
                                                                            r * 94.0F / 255.0F * Y_SIDE_SHADE * ambientLightFactor,
                                                                            g * 157.0F / 255.0F * Y_SIDE_SHADE * ambientLightFactor,
                                                                            b * 52.0F / 255.0F * Y_SIDE_SHADE * ambientLightFactor, a,
                                                                            IndexTextures[index], 0.0F, 1.0F, 0.0F, brightness));
        const uint16_t triangleIndex3 = tessellationHelper.AddVertex(Vertex(x + BlockBounds.GetMaxX(), y + BlockBounds.GetMaxY(),
                                                                            z + BlockBounds.GetMinZ(), 1, 0,
                                                                            94.0F / 255.0F * Y_SIDE_SHADE * ambientLightFactor,
                                                                            157.0F / 255.0F * Y_SIDE_SHADE * ambientLightFactor,
                                                                            52.0F / 255.0F * Y_SIDE_SHADE * ambientLightFactor, a,
                                                                            IndexTextures[index], 0.0F, 1.0F, 0.0F, brightness));
        const uint16_t triangleIndex4 = tessellationHelper.AddVertex(Vertex(x + BlockBounds.GetMaxX(), y + BlockBounds.GetMaxY(),
                                                                            z + BlockBounds.GetMaxZ(), 1, 1,
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
        Block::GenerateTessellationData(tessellationHelper, face, x, y, z, brightness, r, g, b, a);
    }
}

GrassBlock::GrassBlock()
{
    IndexTextures.push_back(EngineDefaults::RegisterTexture(Texture::LoadTexture("Textures/Blocks/GrassTop.png")));
    IndexTextures.push_back(EngineDefaults::RegisterTexture(Texture::LoadTexture("Textures/Blocks/GrassSide.png")));
    IndexTextures.push_back(EngineDefaults::RegisterTexture(Texture::LoadTexture("Textures/Blocks/Dirt.png")));
}

void GrassBlock::Tick(World* world, int x, int y, int z) const
{
    Block::Tick(world, x, y, z);
    if (world->GetBrightnessAt(x, y + 1, z) == 1)
    {
        for (int i = 0; i < 4; i++)
        {
            const int targetX = x + world->RandomEngineState.GetNext(-1, 2);
            const int targetY = y + world->RandomEngineState.GetNext(-3, 2);
            if (const int targetZ = z + world->RandomEngineState.GetNext(-1, 2); world->GetBlockTypeAt(targetX, targetY, targetZ) == EBlockType::Dirt && world->GetBrightnessAt(targetX, targetY, targetZ) == 1)
            {
                world->PlaceBlockAt(targetX, targetY, targetZ, EBlockType::Grass);
            }
        }
    }
    else
    {
        world->PlaceBlockAt(x, y, z, EBlockType::Dirt);
    }
}
