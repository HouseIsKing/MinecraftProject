#include "PlanksBlockRenderer.h"
#include "Util/EngineDefaultsClient.h"

PlanksBlockRenderer::PlanksBlockRenderer()
{
    IndexTextures.push_back(EngineDefaultsClient::RegisterTexture(Texture::LoadTexture("Textures/Blocks/Planks.png")));
}

size_t PlanksBlockRenderer::GetIndexTextureSide(BlockFaces /*face*/) const
{
    return 0;
}
