//
// Created by amit on 4/22/2022.
//
#pragma once
#include "../Shaders/Shader.h"
#include "../World/Blocks/Block.h"
#include <array>
#include "../World/Chunk.h"
#include "CustomRandomEngine.h"

using std::array;
using std::unique_ptr;

class EngineDefaults {
private:
    static unique_ptr<Shader> theShader;
    static bool hasInit;
    static void init();
    static CustomRandomEngine engine;
public:
    static Shader* getShader();
	template<typename T>
    static T getNext(T maxValue);
    template<typename T>	
    static T getNext(T minValue, T maxValue);
    static int getChunkLocalIndex(int x, int y, int z);
};

template<typename T>
inline T EngineDefaults::getNext(T maxValue)
{
    return getNext<T>(0, maxValue);
}

template<typename T>
inline T EngineDefaults::getNext(T minValue, T maxValue)
{
    T modulu = maxValue - minValue;
	T next = engine.getNext() % modulu;
	return next + minValue;
}
