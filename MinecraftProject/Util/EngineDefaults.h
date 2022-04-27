//
// Created by amit on 4/22/2022.
//

#ifndef MINECRAFTPROJECT_ENGINEDEFAULTS_H
#define MINECRAFTPROJECT_ENGINEDEFAULTS_H
#include "../Shaders/Shader.h"
#include "../World/Blocks/Block.h"
#include <array>
#include "../World/Chunk.h"

using std::array;
using std::unique_ptr;

class EngineDefaults {
private:
    static unique_ptr<Shader> theShader;
    static bool hasInit;
    static void init();
public:
    static Shader* getShader();
    static int getChunkLocalIndex(int x, int y, int z);
};

struct PosBlock
{
public:
    int x;
    int y;
    int z;
    EBlockType type;
    PosBlock(int x, int y, int z, EBlockType type);
};


#endif //MINECRAFTPROJECT_ENGINEDEFAULTS_H
