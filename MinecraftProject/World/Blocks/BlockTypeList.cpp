//
// Created by amit on 4/22/2022.
//

#include "BlockTypeList.h"
#include "GrassBlock.h"
#include "AirBlock.h"

using std::piecewise_construct;
using std::forward_as_tuple;

const Block* BlockTypeList::getBlockTypeData(EBlockType type)
{
    initBlockTypes();
    return blockTypes.at(type).get();
}

void BlockTypeList::initBlockTypes() {
    if(init)
        return;
    init = true;
    blockTypes.clear();
    blockTypes.emplace(piecewise_construct, forward_as_tuple(EBlockType::AIR), forward_as_tuple(new AirBlock()));
    blockTypes.emplace(piecewise_construct, forward_as_tuple(EBlockType::GRASS), forward_as_tuple(new GrassBlock()));
}

void BlockTypeList::resetBlockTypes() {
    init = false;
    initBlockTypes();
}

bool BlockTypeList::init = false;

unordered_map<EBlockType, unique_ptr<Block>> BlockTypeList::blockTypes{};
