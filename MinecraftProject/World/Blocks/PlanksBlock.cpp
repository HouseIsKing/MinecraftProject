#include "PlanksBlock.h"

#include "Util/EngineDefaults.h"

PlanksBlock::PlanksBlock()
{
    IndexTextures.push_back(EngineDefaults::RegisterTexture(Texture::LoadTexture("Textures/Blocks/Planks.png")));
}

size_t PlanksBlock::GetIndexTextureSide(BlockFaces /*face*/) const
{
    return 0;
}
