#include "StoneBlockRenderer.h"
#include "Util/EngineDefaultsClient.h"

StoneBlockRenderer::StoneBlockRenderer()
{
    IndexTextures.push_back(EngineDefaultsClient::RegisterTexture(Texture::LoadTexture("Textures/Blocks/Stone.png")));
}

size_t StoneBlockRenderer::GetIndexTextureSide(BlockFaces /*face*/) const
{
    return 0;
}
