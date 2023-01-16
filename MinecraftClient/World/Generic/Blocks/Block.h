#pragma once
#include "Util/BlockFaces.h"
#include "Util/BoundingBox.h"
#include "Util/GuiTessellation.h"
#include "Util/TessellationHelper.h"

template <class T>
class ParticleEntity;

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
    std::vector<uint16_t> IndexTextures;
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
    virtual void GenerateTessellationDataForAllFaces(TessellationHelper& tessellationHelper, const float& x, const float& y, const float& z, const int& brightness, float r = 1.0F, float g = 1.0F, float b = 1.0F, float a = 1.0F) const;
    virtual void GenerateTessellationData(TessellationHelper& tessellationHelper, const BlockFaces& face, const float& x, const float& y, const float& z, const int& brightness, float r = 1.0F, float g = 1.0F, float b = 1.0F, float a = 1.0F) const;
    [[nodiscard]] virtual size_t GetIndexTextureSide(BlockFaces face) const = 0;
    virtual void GenerateTessellationData(GuiTessellation& tessellationHelper, const BlockFaces& face, glm::mat4x4 transformationMatrix) const;
    template <typename T>
    void Tick(T* world, int x, int y, int z) const;
    template <typename T>
    void OnBreak(T* world, int x, int y, int z) const;
    [[nodiscard]] uint16_t GetTextureFromIndex(const size_t& index) const;
    [[nodiscard]] virtual DrawType GetDrawType() const;
    [[nodiscard]] virtual bool IsSolidBlock() const;
    [[nodiscard]] virtual bool IsBlockingLight() const;
};

template <typename T>
void Block::Tick(T* world, int x, int y, int z) const
{
}

template <typename T>
void Block::OnBreak(T* world, int x, int y, int z) const
{
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            for (int k = 0; k < 4; k++)
            {
                const float posX = static_cast<float>(x) + (static_cast<float>(i) + 0.5F) / 4.0F;
                const float posY = static_cast<float>(y) + (static_cast<float>(j) + 0.5F) / 4.0F;
                const float posZ = static_cast<float>(z) + (static_cast<float>(k) + 0.5F) / 4.0F;
                new ParticleEntity<T>(posX, posY, posZ, posX - static_cast<float>(x) - 0.5F, posY - static_cast<float>(y) - 0.5F, posZ - static_cast<float>(z) - 0.5F, this);
            }
        }
    }
}
