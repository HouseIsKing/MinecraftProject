//
// Created by amit on 4/22/2022.
//

#ifndef MINECRAFTPROJECT_GRASSBLOCK_H
#define MINECRAFTPROJECT_GRASSBLOCK_H
#include "Block.h"
#include "../../Textures/Texture.h"

using std::unique_ptr;

class GrassBlock : public Block {
private:
    static unique_ptr<vector<Texture>> textures;
public:
    static const vector<Texture> &getTextures();
    [[nodiscard]] Texture getFaceTexture(BlockFaces face) const override;
    GrassBlock(float x, float y,float z);
};

#endif //MINECRAFTPROJECT_GRASSBLOCK_H
