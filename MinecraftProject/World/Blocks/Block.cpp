//
// Created by amit on 4/22/2022.
//

#include "Block.h"

#include <utility>

Block::Block() : Block(0,0,0,1,1,1) {
}

Block::Block(float minX, float minY, float minZ, float maxX, float maxY, float maxZ) : boundingBox(minX, minY, minZ, maxX, maxY, maxZ) {

}

Block::~Block()
{
}

BoundingBox Block::getBoundingBox() const {
    return boundingBox;
}