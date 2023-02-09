#include "DirtBlockRenderer.h"
#include "Util/EngineDefaultsClient.h"

size_t DirtBlockRenderer::GetIndexTextureSide(BlockFaces /*face*/) const
{
    return 0;
}

DirtBlockRenderer::DirtBlockRenderer()
{
    IndexTextures.push_back(EngineDefaultsClient::RegisterTexture(Texture::LoadTexture("Textures/Blocks/Dirt.png")));
}
