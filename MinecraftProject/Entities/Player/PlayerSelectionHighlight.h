#pragma once
#include "Util/TessellationHelper.h"
#include "World/Blocks/Block.h"

class PlayerSelectionHighlight
{
    TessellationHelper Tessellation;
    uint16_t TextureIndex;
public:
    PlayerSelectionHighlight();
    void Reset();
    const Block* BlockHit;
    BlockFaces FaceHit;
    glm::ivec3 HitPosition;
    void Render(const float& brightness);
};
