#pragma once
#include "CustomRandomEngine.h"

class EngineDefaults
{
    static CustomRandomEngine Engine;

public:
    static float ConvertLightLevelToAmbient(int lightLevel);
    template <typename T>
    static T GetNext(T maxValue);
    template <typename T>
    static T GetNext(T minValue, T maxValue);
    static float GetNextFloat();
    static int GetChunkLocalIndex(int x, int y, int z);
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
