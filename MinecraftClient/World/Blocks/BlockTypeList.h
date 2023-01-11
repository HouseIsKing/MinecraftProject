#pragma once
#include "../../Textures/Texture.h"
#include <memory>
#include <unordered_map>

class GrassBlock;
class Block;

using std::unique_ptr;
using std::unordered_map;

enum class EBlockType : uint8_t
{
    Air = 0,
    Stone = 1,
    Grass = 2,
    Dirt = 3,
    Cobblestone = 4,
    Planks = 5,
    Sapling = 6
};

template <>
struct std::hash<EBlockType>
{
    size_t operator()(const EBlockType& blockType) const noexcept
    {
        return static_cast<size_t>(blockType);
    }
};

std::istream& operator>>(std::istream& is, EBlockType& blockType);

class BlockTypeList
{
    static unordered_map<EBlockType, unique_ptr<Block>> BlockTypes;
    static bool Init;

public:
    static const Block* GetBlockTypeData(EBlockType type);
    static void InitBlockTypes();
    static void ResetBlockTypes();
};
