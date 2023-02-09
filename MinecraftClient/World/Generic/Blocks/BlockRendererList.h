#pragma once
#include "BlockRenderer.h"
#include "Util/EngineDefaults.h"
#include <memory>
#include <unordered_map>

class BlockRendererList
{
    static std::unordered_map<EBlockType, std::unique_ptr<BlockRenderer>, BlockTypeHasher> BlockTypes;
    static bool Init;

public:
    static const BlockRenderer* GetBlockRenderer(EBlockType type);
    static void InitBlockTypes();
    static void ResetBlockTypes();
};
