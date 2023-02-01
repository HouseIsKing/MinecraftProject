#pragma once
#include <cstdint>
#include <istream>
#include <unordered_map>

#include "Block.h"
#include "Util/EngineDefaults.h"

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
