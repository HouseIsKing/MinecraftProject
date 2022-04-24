//
// Created by amit on 4/22/2022.
//

#ifndef MINECRAFTPROJECT_BLOCKTYPELIST_H
#define MINECRAFTPROJECT_BLOCKTYPELIST_H
#include "../../Textures/Texture.h"
#include <vector>
#include <memory>

class GrassBlock;

using std::vector;
using std::unique_ptr;

enum class EBlockType{
    AIR,GRASS,COBBLESTONE
};

struct BlockType {
private:
    EBlockType type;
    vector<Texture> textures;
public:
    BlockType(EBlockType type, vector<Texture> textures);
    [[nodiscard]] EBlockType getType() const;
    [[nodiscard]] const vector<Texture> &getTextures() const;
};

class BlockTypeList {
private:
    static unordered_map<EBlockType, unique_ptr<BlockType>> blockTypes;
    static bool init;
public:
    static const BlockType* getBlockType(EBlockType type);
    static void initBlockTypes();
    static void resetBlockTypes();
};


#endif //MINECRAFTPROJECT_BLOCKTYPELIST_H
