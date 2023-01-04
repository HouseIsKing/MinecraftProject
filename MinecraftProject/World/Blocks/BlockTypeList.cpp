//
// Created by amit on 4/22/2022.
//

#include "BlockTypeList.h"
#include "../../Util/EngineDefaults.h"
#include "AirBlock.h"
#include "CobblestoneBlock.h"
#include "DirtBlock.h"
#include "GrassBlock.h"
#include "PlanksBlock.h"
#include "StoneBlock.h"

using std::piecewise_construct;
using std::forward_as_tuple;

std::istream& operator>>(std::istream& is, EBlockType& blockType)
{
    uint8_t temp = 0;
    is >> temp;
    blockType = static_cast<EBlockType>(temp);
    return is;
}

const Block* BlockTypeList::GetBlockTypeData(const EBlockType type)
{
    InitBlockTypes();
    return BlockTypes.at(type).get();
}

void BlockTypeList::InitBlockTypes()
{
    if (Init)
    {
        return;
    }
    Init = true;
    BlockTypes.clear();
    BlockTypes.emplace(EBlockType::Air, new AirBlock());
    BlockTypes.emplace(EBlockType::Grass, new GrassBlock());
    BlockTypes.emplace(EBlockType::Dirt, new DirtBlock());
    BlockTypes.emplace(EBlockType::Cobblestone, new CobblestoneBlock());
    BlockTypes.emplace(EBlockType::Stone, new StoneBlock());
    BlockTypes.emplace(EBlockType::Planks, new PlanksBlock());
}

void BlockTypeList::ResetBlockTypes()
{
    Init = false;
    InitBlockTypes();
}

bool BlockTypeList::Init = false;
unordered_map<EBlockType, unique_ptr<Block>> BlockTypeList::BlockTypes{};
