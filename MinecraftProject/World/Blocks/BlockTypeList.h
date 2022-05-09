//
// Created by amit on 4/22/2022.
//

#pragma once
#include "../../Textures/Texture.h"
#include <memory>
#include <unordered_map>

class GrassBlock;
class Block;

using std::unique_ptr;
using std::unordered_map;

enum class EBlockType
{
    Air,
    Grass
};

template <>
struct std::hash<EBlockType>
{
    size_t operator()(const EBlockType& blockType) const noexcept
    {
        return static_cast<size_t>(blockType);
    }
};

class BlockTypeList
{
    static unordered_map<EBlockType, unique_ptr<Block>> BlockTypes;
    static std::vector<Texture*> TextureList;
    static bool Init;
    static GLuint UBO;
public:
    static const Block* GetBlockTypeData(EBlockType type);
    static uint16_t RegisterTexture(Texture* texture);
    static void InitBlockTypes();
    static void ResetBlockTypes();
};
