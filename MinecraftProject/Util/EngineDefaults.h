//
// Created by amit on 4/22/2022.
//
#pragma once
#include "../Shaders/Shader.h"
#include "CustomRandomEngine.h"
#include <array>

using std::array;
using std::unique_ptr;

class EngineDefaults
{
    static unique_ptr<Shader> TheShader;
    static bool HasInit;
    static void Init();
    static CustomRandomEngine Engine;
public:
    static Shader* GetShader();
    template <typename T>
    static T GetNext(T maxValue);
    template <typename T>
    static T GetNext(T minValue, T maxValue);
    static int GetChunkLocalIndex(int x, int y, int z);
    static int GetLocalIndex2D(int x, int z);
};

template <typename T>
T EngineDefaults::GetNext(T maxValue)
{
    return GetNext<T>(0, maxValue);
}

template <typename T>
T EngineDefaults::GetNext(T minValue, T maxValue)
{
    T modulo = maxValue - minValue;
    T next = Engine.GetNext() % modulo;
    return next + minValue;
}
