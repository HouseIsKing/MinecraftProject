//
// Created by amit on 4/22/2022.
//

#pragma once
#include "../../Util/BoundingBox.h"
#include "../../Util/TessellationHelper.h"
#include <array>

#include "Util/GuiTessellation.h"
class SinglePlayerWorld;
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

enum class DrawType
{
    Default,
    Sapling
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
    void GenerateTessellationData(TessellationHelper& tessellationHelper, const BlockFaces& face, const float& x, const float& y, const float& z, const int& brightness) const;
    virtual void GenerateTessellationDataForAllFaces(TessellationHelper& tessellationHelper, const float& x, const float& y, const float& z, const int& brightness, float r, float g, float b, float a) const;
    virtual void GenerateTessellationData(TessellationHelper& tessellationHelper, const BlockFaces& face, const float& x, const float& y, const float& z, const int& brightness, float r, float g, float b, float a) const;
    [[nodiscard]] virtual size_t GetIndexTextureSide(BlockFaces face) const = 0;
    virtual void GenerateTessellationData(GuiTessellation& tessellationHelper, const BlockFaces& face, mat4x4 transformationMatrix) const;
    virtual void Tick(SinglePlayerWorld* world, int x, int y, int z) const;
    virtual void OnBreak(SinglePlayerWorld* world, int x, int y, int z) const;
    [[nodiscard]] uint16_t GetTextureFromIndex(const size_t& index) const;
    [[nodiscard]] virtual DrawType GetDrawType() const;
    [[nodiscard]] virtual bool IsSolidBlock() const;
    [[nodiscard]] virtual bool IsBlockingLight() const;
};
