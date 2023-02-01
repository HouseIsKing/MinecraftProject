#pragma once
#include "Block.h"
#include "Util/EngineDefaults.h"

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
    static std::unordered_map<EBlockType, std::unique_ptr<Block>> BlockTypes;
    static bool Init;

public:
    static const Block* GetBlockTypeData(EBlockType type);
    static void InitBlockTypes();
    static void ResetBlockTypes();
};
