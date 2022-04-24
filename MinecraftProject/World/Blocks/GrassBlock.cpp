//
// Created by amit on 4/22/2022.
//

#include "GrassBlock.h"

#include <memory>

unique_ptr<vector<Texture>> GrassBlock::textures = nullptr;

const vector<Texture> &GrassBlock::getTextures() {
    textures = std::make_unique<vector<Texture>>(vector<Texture>{Texture::loadTexture("Textures/Blocks/GrassTop.png"),
                                 Texture::loadTexture("Textures/Blocks/GrassSide.png"),
                                 Texture::loadTexture("Textures/Blocks/Dirt.png")});
    return *textures;
}

Texture GrassBlock::getFaceTexture(BlockFaces face) const {
    switch(face) {
        case BlockFaces::TOP:
            return (*textures)[0];
        case BlockFaces::BOTTOM:
            return (*textures)[2];
        case BlockFaces::WEST:
        case BlockFaces::EAST:
        case BlockFaces::NORTH:
        case BlockFaces::SOUTH:
        default:
            return (*textures)[1];
    }
}

GrassBlock::GrassBlock(float x, float y, float z) : Block(BlockTypeList::getBlockType(EBlockType::GRASS), x, y, z) {
}
