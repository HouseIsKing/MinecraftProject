//
// Created by amit on 4/22/2022.
//

#pragma once
#include "../../Util/BoundingBox.h"
#include "../../Util/TessellationHelper.h"
#include "BlockTypeList.h"
#include <array>

using std::array;
using glm::floor;

enum class BlockFaces
{
    Top,
    Bottom,
    West,
    East,
    North,
    South
};

class Block
{
protected:
    static constexpr float X_SIDE_SHADE = 0.6F;
    static constexpr float Z_SIDE_SHADE = 0.8F;
    static constexpr float Y_SIDE_SHADE = 1.0F;
    vector<uint16_t> IndexTextures;
    BoundingBox BlockBounds;
    Block();
    Block(float minX, float minY, float minZ, float maxX, float maxY, float maxZ);
public:
    virtual ~Block();
    Block(const Block&) = delete;
    Block& operator=(const Block&) = delete;
    Block(Block&&) = delete;
    Block& operator=(Block&&) = delete;
    [[nodiscard]] BoundingBox GetBoundingBox() const;
    virtual void GenerateTessellationData(TessellationHelper& tessellationHelper, const BlockFaces& face, const float& x, const float& y, const float& z, const int& brightness) const = 0;
};
