//
// Created by amit on 4/22/2022.
//

#pragma once
#include "../../Util/BoundingBox.h"
#include "BlockTypeList.h"
#include "../../Util/TessellationHelper.h"
#include <array>

using std::array;
using glm::floor;

enum class BlockFaces {
    TOP,
    BOTTOM,
    WEST,
    EAST,
    NORTH,
    SOUTH
};

class Block {
protected:
    vector<uint16_t> IndexTextures;
    BoundingBox boundingBox;
    Block();
    Block(float minX, float minY, float minZ, float maxX, float maxY, float maxZ);
public:
    virtual ~Block();
    BoundingBox getBoundingBox() const;
    virtual void generateTessellationData(TessellationHelper& tessellationHelper, const BlockFaces& face, const float& x, const float& y, const float& z) const = 0;
};
