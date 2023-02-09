#include "CobblestoneBlockRenderer.h"
#include "Util/EngineDefaultsClient.h"

size_t CobblestoneBlockRenderer::GetIndexTextureSide(BlockFaces /*face*/) const
{
	return 0;
}

CobblestoneBlockRenderer::CobblestoneBlockRenderer()
{
	IndexTextures.push_back(EngineDefaultsClient::RegisterTexture(Texture::LoadTexture("Textures/Blocks/Cobblestone.png")));
}
