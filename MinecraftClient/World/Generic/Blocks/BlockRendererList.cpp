#include "BlockRendererList.h"
#include "AirBlockRenderer.h"
#include "CobblestoneBlockRenderer.h"
#include "DirtBlockRenderer.h"
#include "GrassBlockRenderer.h"
#include "PlanksBlockRenderer.h"
#include "SaplingBlockRenderer.h"
#include "StoneBlockRenderer.h"

const BlockRenderer* BlockRendererList::GetBlockRenderer(const EBlockType type)
{
    return BlockTypes.at(type).get();
}

void BlockRendererList::InitBlockTypes()
{
    if (Init)
    {
        return;
    }
    Init = true;
    BlockTypes.clear();
    BlockTypes.emplace(EBlockType::Air, new AirBlockRenderer());
    BlockTypes.emplace(EBlockType::Grass, new GrassBlockRenderer());
    BlockTypes.emplace(EBlockType::Dirt, new DirtBlockRenderer());
    BlockTypes.emplace(EBlockType::Cobblestone, new CobblestoneBlockRenderer());
    BlockTypes.emplace(EBlockType::Stone, new StoneBlockRenderer());
    BlockTypes.emplace(EBlockType::Planks, new PlanksBlockRenderer());
    BlockTypes.emplace(EBlockType::Sapling, new SaplingBlockRenderer());
}

void BlockRendererList::ResetBlockTypes()
{
    Init = false;
    InitBlockTypes();
}

bool BlockRendererList::Init = false;
std::unordered_map<EBlockType, std::unique_ptr<BlockRenderer>, BlockTypeHasher> BlockRendererList::BlockTypes{};
