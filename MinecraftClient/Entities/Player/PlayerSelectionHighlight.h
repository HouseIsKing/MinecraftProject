#pragma once
#include "World/Blocks/Block.h"

class PlayerController;

class PlayerSelectionHighlight
{
    TessellationHelper Tessellation;
    uint16_t TextureIndex;
    PlayerController* Player;

public:
    explicit PlayerSelectionHighlight(Player* player);
    void Reset();
    const Block* BlockHit;
    BlockFaces FaceHit;
    glm::ivec3 HitPosition;
    void Render(const int& brightness);
};
