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
using std::shared_ptr;
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
private:
    static shared_ptr<Block> airBlock;
    static bool initAirBlock;
    BoundingBox boundingBox;
    array<float,24> UVs;
    bool UVsOverride;
protected:
    const BlockType* type;
    Block(const BlockType* type, float x, float y, float z);
    Block(const BlockType* type, float minX, float minY, float minZ, float maxX, float maxY, float maxZ);
    Block(const BlockType* type, float minX, float minY, float minZ, float maxX, float maxY, float maxZ, array<float,24> UVs);
    [[nodiscard]] virtual Texture getFaceTexture(BlockFaces face) const;
public:
    [[nodiscard]] bool isAirBlock() const;
    BoundingBox& getBoundingBox();
    void generateTessellationData(TessellationHelper& tessellationHelper, BlockFaces face);
    static Block* getAirBlock();
};


#endif //MINECRAFTPROJECT_BLOCK_H
