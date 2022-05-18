#pragma once
#include "Util/TessellationHelper.h"
#include "World/Blocks/Block.h"

class PlayerSelectionHighlight
{
    TessellationHelper Tessellation;
    uint16_t TextureIndex;
public:
    PlayerSelectionHighlight();
    void Render(const float& x, const float& y, const float& z, const BlockFaces& face, const float& brightness, const Block& blockToDrawUpon);
};
