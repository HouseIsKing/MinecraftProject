#pragma once
#include "Block.h"
#include "Util/EngineDefaults.h"
#include <memory>
#include <unordered_map>

class BlockTypeList
{
    static std::unordered_map<EBlockType, std::unique_ptr<Block>, BlockTypeHasher> BlockTypes;
    static bool Init;

public:
    static const Block* GetBlockTypeData(EBlockType type);
    static void InitBlockTypes();
    static void ResetBlockTypes();
};
