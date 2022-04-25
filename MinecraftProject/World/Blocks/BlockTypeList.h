//
// Created by amit on 4/22/2022.
//

#ifndef MINECRAFTPROJECT_BLOCKTYPELIST_H
#define MINECRAFTPROJECT_BLOCKTYPELIST_H
#include <memory>
#include <unordered_map>

class GrassBlock;
class Block;

using std::unique_ptr;
using std::unordered_map;

enum class EBlockType{
    AIR,GRASS
};

template<>
struct std::hash<EBlockType> {
    size_t operator()(const EBlockType& blockType) const {
        return (size_t)blockType;
    }
};

class BlockTypeList {
private:
    static unordered_map<EBlockType, unique_ptr<Block>> blockTypes;
    static bool init;
public:
    static const Block* getBlockTypeData(EBlockType type);
    static void initBlockTypes();
    static void resetBlockTypes();
};


#endif //MINECRAFTPROJECT_BLOCKTYPELIST_H
