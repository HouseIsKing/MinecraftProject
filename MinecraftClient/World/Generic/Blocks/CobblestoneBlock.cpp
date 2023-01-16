#include "CobblestoneBlock.h"

#include "Util/EngineDefaults.h"

size_t CobblestoneBlock::GetIndexTextureSide(BlockFaces /*face*/) const
{
	return 0;
}

CobblestoneBlock::CobblestoneBlock()
{
	IndexTextures.push_back(EngineDefaults::RegisterTexture(Texture::LoadTexture("Textures/Blocks/Cobblestone.png")));
}
