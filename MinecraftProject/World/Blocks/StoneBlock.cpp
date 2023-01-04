#include "StoneBlock.h"

#include "Util/EngineDefaults.h"

StoneBlock::StoneBlock()
{
    IndexTextures.push_back(EngineDefaults::RegisterTexture(Texture::LoadTexture("Textures/Blocks/Stone.png")));
}

size_t StoneBlock::GetIndexTextureSide(BlockFaces face) const
{
    return 0;
}
