//
// Created by amit on 4/22/2022.
//

#include "BlockTypeList.h"
#include "GrassBlock.h"

using std::piecewise_construct;
using std::forward_as_tuple;

BlockType::BlockType(EBlockType type, vector<Texture> textures) : type(type), textures(std::move(textures)) {}

EBlockType BlockType::getType() const {
    return type;
}

const vector<Texture> &BlockType::getTextures() const {
    return textures;
}

void BlockTypeList::initBlockTypes() {
    if(init)
        return;
    init = true;
    blockTypes.clear();
    blockTypes.emplace(piecewise_construct, forward_as_tuple(EBlockType::AIR), forward_as_tuple(new BlockType(EBlockType::AIR, vector<Texture>())));
    blockTypes.emplace(piecewise_construct, forward_as_tuple(EBlockType::GRASS), forward_as_tuple(new BlockType(EBlockType::GRASS, GrassBlock::getTextures())));
}

const BlockType* BlockTypeList::getBlockType(EBlockType type) {
    initBlockTypes();
    return blockTypes.at(type).get();
}

void BlockTypeList::resetBlockTypes() {
    init = false;
    initBlockTypes();
}

bool BlockTypeList::init = false;

unordered_map<EBlockType, unique_ptr<BlockType>> BlockTypeList::blockTypes{};
