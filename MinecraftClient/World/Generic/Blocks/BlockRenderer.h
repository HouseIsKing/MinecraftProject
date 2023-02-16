#pragma once
#include "Util/GuiTessellation.h"
#include "Util/TessellationHelper.h"
#include "World/Blocks/Block.h"

class BlockRenderer
{
protected:
    static constexpr float X_SIDE_SHADE = 0.6F;
    static constexpr float Z_SIDE_SHADE = 0.8F;
    static constexpr float Y_SIDE_SHADE = 1.0F;
    std::vector<uint16_t> IndexTextures;
    BlockRenderer() = default;

public:
    virtual ~BlockRenderer() = default;
    BlockRenderer(const BlockRenderer&) = delete;
    BlockRenderer(BlockRenderer&&) = delete;
    BlockRenderer& operator=(const BlockRenderer&) = delete;
    BlockRenderer& operator=(BlockRenderer&&) = delete;
    virtual void GenerateTessellationDataForAllFaces(const Block* block, TessellationHelper& tessellationHelper, const float& x, const float& y, const float& z, const int& brightness, float r = 1.0F, float g = 1.0F, float b = 1.0F, float a = 1.0F) const;
    virtual void GenerateTessellationData(const Block* block, TessellationHelper& tessellationHelper, const BlockFaces& face, const float& x, const float& y, const float& z, const int& brightness, float r = 1.0F, float g = 1.0F, float b = 1.0F, float a = 1.0F) const;
    virtual void GenerateTessellationData(const Block* block, GuiTessellation& tessellationHelper, const BlockFaces& face, glm::mat4x4 transformationMatrix) const;
    [[nodiscard]] virtual DrawType GetDrawType() const;
    [[nodiscard]] virtual size_t GetIndexTextureSide(BlockFaces face) const = 0;
    [[nodiscard]] uint16_t GetTextureFromIndex(size_t textureIndex) const;
};
