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
    static Block emptyBlocksArray[Chunk::CHUNK_WIDTH][Chunk::CHUNK_HEIGHT][Chunk::CHUNK_DEPTH];
public:
    static Shader* getShader();
    static int getChunkLocalIndex(int x, int y, int z);
};


#endif //MINECRAFTPROJECT_ENGINEDEFAULTS_H
