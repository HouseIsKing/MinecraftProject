//
// Created by amit on 4/22/2022.
//

#ifndef MINECRAFTPROJECT_GRASSBLOCK_H
#define MINECRAFTPROJECT_GRASSBLOCK_H
#include "Block.h"
#include "../../Textures/Texture.h"

using std::unique_ptr;

class GrassBlock : public Block {
public:
    void generateTessellationData(TessellationHelper& tessellationHelper, const BlockFaces& face, const float& x, const float& y, const float& z) const override;
    GrassBlock();
};

#endif //MINECRAFTPROJECT_GRASSBLOCK_H
