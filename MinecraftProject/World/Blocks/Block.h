//
// Created by amit on 4/22/2022.
//

#ifndef MINECRAFTPROJECT_BLOCK_H
#define MINECRAFTPROJECT_BLOCK_H
#include "../../Util/BoundingBox.h"
#include "BlockTypeList.h"
#include "../../Util/TessellationHelper.h"
#include <array>
#include <memory>

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
    vector<Texture*> textures;
    BoundingBox boundingBox;
    Block();
    Block(float minX, float minY, float minZ, float maxX, float maxY, float maxZ);
public:
    virtual ~Block();
    BoundingBox getBoundingBox() const;
    virtual void generateTessellationData(TessellationHelper& tessellationHelper, const BlockFaces& face, const float& x, const float& y, const float& z) const = 0;
};


#endif //MINECRAFTPROJECT_BLOCK_H
