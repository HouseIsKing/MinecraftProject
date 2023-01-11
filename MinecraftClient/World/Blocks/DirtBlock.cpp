#include "DirtBlock.h"

#include "Util/EngineDefaults.h"

size_t DirtBlock::GetIndexTextureSide(BlockFaces /*face*/) const
{
    return 0;
}

DirtBlock::DirtBlock()
{
    IndexTextures.push_back(EngineDefaults::RegisterTexture(Texture::LoadTexture("Textures/Blocks/Dirt.png")));
}
